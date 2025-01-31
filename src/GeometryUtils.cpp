#include "GeometryUtils.h"

static CGAL::Random randomGenerator; // Static instance, initialized once

std::vector<Point> GeometryUtils::getIntersectionShape(const Tetrahedron& T1, const Tetrahedron& T2) {
    std::vector<Point> resulting_shape;
    if(!checkIntersection(T1, T2)) return resulting_shape;

    Nef_polyhedron nef1 (tetrahedronToMesh(T1));
    Nef_polyhedron nef2 (tetrahedronToMesh(T2));

    Nef_polyhedron intersection = nef1 * nef2;

    if(intersection.is_empty()) return resulting_shape; 
    Polyhedron poly;
    intersection.convert_to_polyhedron(poly);

    if (!CGAL::is_valid_polygon_mesh(poly)) {
        throw std::runtime_error("Conversion Failed");
    }

    for(auto vit = poly.vertices_begin(); vit != poly.vertices_end(); ++vit) {
        Point p = vit->point();
        resulting_shape.push_back(p);
    }

    return resulting_shape;
}

double GeometryUtils::getIntersectionVolume(const Tetrahedron& T1, const Tetrahedron& T2) {
    double resulting_volume = 0;
    if(!checkIntersection(T1, T2)) return resulting_volume;

    Nef_polyhedron nef1 (tetrahedronToMesh(T1));
    Nef_polyhedron nef2 (tetrahedronToMesh(T2));

    Nef_polyhedron intersection = nef1 * nef2;

    if(intersection.is_empty()) throw std::runtime_error("Intersection Shouldn't Be Empty Here");
        
    try {
        Nef_polyhedron regularized = intersection.regularization();
        if (regularized.is_simple()) {
            intersection = regularized;
        }
    } catch (const std::exception& e) {
        std::cerr << "Regularization Failed: " << e.what() << std::endl;
    }

    Polyhedron resulting_polyhedron;
    try {
        intersection.convert_to_polyhedron(resulting_polyhedron);
    } catch (const std::exception& e) {
        std::cerr << "Polyhedron Conversion Error: " << e.what() << std::endl;
    }
    
    if (resulting_polyhedron.is_empty()) return resulting_volume;

    if (!CGAL::is_valid_polygon_mesh(resulting_polyhedron)) {
        throw std::runtime_error("Something Went Wrong, Resulting Polyhedron Is Invalid Mesh");
    }

    auto volume = CGAL::Polygon_mesh_processing::volume(resulting_polyhedron);
    resulting_volume = CGAL::to_double(volume);

    return resulting_volume;
}

IntersectionType GeometryUtils::getIntersectionClassification(const Tetrahedron& T1, const Tetrahedron& T2) {
    
    Nef_polyhedron nef1 (tetrahedronToMesh(T1));
    Nef_polyhedron nef2 (tetrahedronToMesh(T2));

    if (nef1.is_empty() || nef2.is_empty()) {
        return IntersectionType::None;
    }

    Nef_polyhedron intersection = nef1 * nef2;
    if (intersection.is_empty()) return IntersectionType::None;

    try {
        Nef_polyhedron regularized = intersection.regularization();
        if (regularized.is_simple()) {
            intersection = regularized;
        }
    } catch (const std::exception& e) {
        std::cerr << "Regularization failed: " << e.what() << std::endl;
    }

    Polyhedron resulting_polyhedron;
    try {
        intersection.convert_to_polyhedron(resulting_polyhedron);
    } catch (const std::exception& e) {
        std::cerr << "Polyhedron conversion error: " << e.what() << std::endl;
    }

    size_t num_vertices = resulting_polyhedron.size_of_vertices();
    size_t num_edges = std::distance(resulting_polyhedron.edges_begin(), resulting_polyhedron.edges_end());
    size_t num_faces = std::distance(resulting_polyhedron.facets_begin(), resulting_polyhedron.facets_end());


    if (num_vertices == 1) {
        return IntersectionType::Point;
    } else if (num_vertices == 2 && num_edges == 1) {
        return IntersectionType::Segment;
    } else if (num_faces > 0) {
        // Euler Characteristic: X = V - E + F
        // X = 1 : Polygons | X = 2 : Polyhedra
        int euler_characteristic = num_vertices - num_edges + num_faces;

        if (euler_characteristic == 1) {
            return IntersectionType::Polygon;
        } else if (euler_characteristic == 2) {
            return IntersectionType::Polyhedron;
        }
    }

    throw std::runtime_error("Something Went Wrong!");
}

bool GeometryUtils::checkIntersection(const Tetrahedron& T1, const Tetrahedron& T2) {
    return CGAL::do_intersect(T1, T2);
}

Mesh GeometryUtils::tetrahedronToMesh(const Tetrahedron& T) {
    Mesh m;

    std::vector<Point> vertices = {T[0], T[1], T[2], T[3]};
    CGAL::convex_hull_3(vertices.begin(), vertices.end(), m);

    if (CGAL::is_closed(m))
        return m;
    else
        throw std::runtime_error("Mesh is open");

    return m;    
}

Point GeometryUtils::generateRandomPoint() {
    double x = randomGenerator.get_double(0.0, 1.0);
    double y = randomGenerator.get_double(0.0, 1.0);
    double z = randomGenerator.get_double(0.0, 1.0);
    return Point(x, y, z);
}

Tetrahedron GeometryUtils::generateRandomTetrahedron(){
    Point vertexA, vertexB, vertexC, vertexD;
    Tetrahedron tetrahedron;

    do {

        vertexA = GeometryUtils::generateRandomPoint();
        vertexB = GeometryUtils::generateRandomPoint();
        vertexC = GeometryUtils::generateRandomPoint();
        vertexD = GeometryUtils::generateRandomPoint();

        tetrahedron = Tetrahedron(vertexA, vertexB, vertexC, vertexD);

    } while (tetrahedron.is_degenerate());

    return tetrahedron;
}

Point GeometryUtils::generateRandomPointOnTriangle(const Point& A, const Point& B, const Point& C) {
    return *CGAL::Random_points_in_triangle_3<Point, CGAL::Creator_uniform_3<double, Point>>(A, B, C);
}

Point GeometryUtils::generateRandomPointOutsideTetrahedron(const Tetrahedron tetrahedron) {
    Point random_point;
    do {
        random_point = generateRandomPoint();
    } while (tetrahedron.has_on_bounded_side(random_point));
    return random_point;
}

GeometryUtils::CoordinateSystem::CoordinateSystem(const Vector& n) {
    // Define local coordinate system
    assert(n.squared_length() > 0 && "Input vector n must be non-zero.");

    
    z_axis = n.direction().vector();
    
    if (std::abs(CGAL::to_double(z_axis.x())) > 0.99) {
        x_axis = CGAL::cross_product(z_axis, Vector(0, 1, 0)); // Define x-axis with fallback if z-axis is nearly aligned with (1, 0, 0)
    } else {
        x_axis = CGAL::cross_product(z_axis, Vector(1, 0, 0));
    }
    x_axis = x_axis / std::sqrt(CGAL::to_double(x_axis.squared_length()));
    
    y_axis = CGAL::cross_product(z_axis, x_axis);
}

double GeometryUtils::CoordinateSystem::calculateMaxRadius(const Point& origin, double theta, double phi) const {
    // Convert unit vector in specified direction to global space
    Point unit_direction = sphericalToGlobal(origin, 1.0, theta, phi);
    
    double x_dir = CGAL::to_double(unit_direction.x() - origin.x());
    double y_dir = CGAL::to_double(unit_direction.y() - origin.y());
    double z_dir = CGAL::to_double(unit_direction.z() - origin.z());
    
    double x_o = CGAL::to_double(origin.x());
    double y_o = CGAL::to_double(origin.y());
    double z_o = CGAL::to_double(origin.z());

    double r_max = std::numeric_limits<double>::max();
    
    // Check each axis constraint
    if (std::abs(x_dir) > epsilon) {
        r_max = std::min(r_max, x_dir > 0 ? (1 - x_o) / x_dir : -x_o / x_dir);
    }
    if (std::abs(y_dir) > epsilon) {
        r_max = std::min(r_max, y_dir > 0 ? (1 - y_o) / y_dir : -y_o / y_dir);
    }
    if (std::abs(z_dir) > epsilon) {
        r_max = std::min(r_max, z_dir > 0 ? (1 - z_o) / z_dir : -z_o / z_dir);
    }
    
    return std::max(epsilon, r_max);
}

Point GeometryUtils::CoordinateSystem::sphericalToGlobal(const Point& origin, double r, double theta, double phi) const {
    assert(r >= 0 && "Radius must be non-negative.");

    // In normal-aligned space:
    // x = r * cos(theta) * sin(phi)
    // y = r * sin(theta) * sin(phi)
    // z = r * cos(phi)
    double x = r * std::cos(theta) * std::sin(phi);
    double y = r * std::sin(theta) * std::sin(phi);
    double z = r * std::cos(phi);

    // Transform to global space using the basis vectors
    Vector offset = (x * x_axis) + (y * y_axis) + (z * z_axis);
    
    return Point(
        CGAL::to_double(origin.x()) + CGAL::to_double(offset.x()),
        CGAL::to_double(origin.y()) + CGAL::to_double(offset.y()),
        CGAL::to_double(origin.z()) + CGAL::to_double(offset.z())
    );
}

