#include "GeometryUtils.h"


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

IntersectionType GeometryUtils::getIntersectionClassification(const Tetrahedron& T1, const Tetrahedron& T2) {
    
    Nef_polyhedron nef1 (tetrahedronToMesh(T1));
    Nef_polyhedron nef2 (tetrahedronToMesh(T2));

    if (nef1.is_empty() || nef2.is_empty()) {
        return IntersectionType::None;
    }

    Nef_polyhedron intersection = nef1 * nef2;
    if (intersection.is_empty()) return IntersectionType::None;

    Polyhedron poly;
    intersection.convert_to_polyhedron(poly);    
    if (!CGAL::is_valid_polygon_mesh(poly)) {
        throw std::runtime_error("Conversion Failed");
    }

    size_t num_vertices = std::distance(poly.vertices_begin(), poly.vertices_end());
    size_t num_edges = std::distance(poly.edges_begin(), poly.edges_end());
    size_t num_faces = std::distance(poly.facets_begin(), poly.facets_end());


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
