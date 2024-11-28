#include "TetrahedronFactory.h"
#include <CGAL/Random.h>
#include <CGAL/enum.h>
#include <chrono>
#include <cmath>
#include <random>
#include "GeometryUtils.h"

static CGAL::Random randomGenerator; // initialized once

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createRandomTetrahedronPair() {

    Tetrahedron tetrahedron1 = GeometryUtils::generateRandomTetrahedron();
    Tetrahedron tetrahedron2 = GeometryUtils::generateRandomTetrahedron();

    return std::make_pair(tetrahedron1, tetrahedron2);
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createRandomTetrahedronPair(int type) {
    if (type == 0) {
        type = rand() % 5 + 1;
    }

    switch (type) {
        case 1:
            return NoIntersection();
        case 2:
            return PointIntersection();
        case 3:
            return LineIntersection();
        case 4:
            return PolygonIntersection();
        case 5:
            return PolyhedronIntersection();
        default:
            throw std::invalid_argument("Invalid tetrahedron type: " + std::to_string(type));
    }
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::NoIntersection() { // No intersection

    Tetrahedron tetrahedron1, tetrahedron2;

    do {
        tetrahedron1 = GeometryUtils::generateRandomTetrahedron();
        tetrahedron2 = GeometryUtils::generateRandomTetrahedron();

    } while (CGAL::do_intersect(tetrahedron1, tetrahedron2));

    return std::make_pair(tetrahedron1, tetrahedron2);
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::PointIntersection() { // Point
    Point vertexA, vertexB, vertexC, vertexD;
    Tetrahedron tetrahedron1, tetrahedron2;

    auto start_time = std::chrono::steady_clock::now();
    constexpr std::chrono::seconds timeout(3);

    std::random_device rd;
    std::mt19937 gen(rd());

    while (true) {
        // Generate the first tetrahedron T1
        tetrahedron1 = GeometryUtils::generateRandomTetrahedron();

        // Generate random point on the selected face
        Point vertex1 = GeometryUtils::generateRandomPointOnTriangle(
            tetrahedron1.vertex(0), tetrahedron1.vertex(1), tetrahedron1.vertex(2)
        );

        // Compute normal and setup coordinate system
        Vector normal = CGAL::normal(tetrahedron1.vertex(0), tetrahedron1.vertex(1), tetrahedron1.vertex(2));

        // Ensure the normal points outward
        Vector to_fourth = tetrahedron1.vertex(3) - tetrahedron1.vertex(0);
        if (CGAL::to_double(normal * to_fourth) > 0) {
            normal = -normal;
        }

        GeometryUtils::CoordinateSystem coords(normal);

        auto attempt_start_time = std::chrono::steady_clock::now();
        bool success = false;

        // Distribution for spherical coordinates
        std::uniform_real_distribution<> dist_theta(-(M_PI/2) + epsilon, (M_PI/2) - epsilon);
        std::uniform_real_distribution<> dist_phi(-(M_PI/2) + epsilon, (M_PI/2) - epsilon);

        while (std::chrono::steady_clock::now() - attempt_start_time < timeout) {
            std::vector<Point> new_vertices = {vertex1};
            bool valid_points = true;

            // Generate three more vertices for T2
            for (int i = 0; i < 3; ++i) {
                double theta = dist_theta(gen);
                double phi = dist_phi(gen);
                
                // Calculate maximum allowable radius for this direction
                double r_max = coords.calculateMaxRadius(vertex1, theta, phi);

                // Generate radius within allowable range
                std::uniform_real_distribution<> dist_r(epsilon, r_max);
                double r = dist_r(gen);

                // Generate point in global space
                Point new_vertex = coords.sphericalToGlobal(vertex1, r, theta, phi);
                
                // Validate point is not inside T1
                if (tetrahedron1.has_on_bounded_side(new_vertex)) {
                    valid_points = false;
                    break;
                }

                new_vertices.push_back(new_vertex);
            }

            if (!valid_points) continue;

            tetrahedron2 = Tetrahedron(
                new_vertices[0], new_vertices[1], 
                new_vertices[2], new_vertices[3]
            );

            if ( !tetrahedron2.is_degenerate() && CGAL::do_intersect(tetrahedron1, tetrahedron2)) {
                success = true;
                break;
            }
        }

        if (success) {
            return std::make_pair(tetrahedron1, tetrahedron2);
        }
    }
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::LineIntersection() { // Line
    Tetrahedron tetrahedron1, tetrahedron2;
    auto start_time = std::chrono::steady_clock::now();
    constexpr std::chrono::seconds timeout(3);

    std::random_device rd;
    std::mt19937 gen(rd());

    while (true) {
        // Generate the first tetrahedron T1
        tetrahedron1 = GeometryUtils::generateRandomTetrahedron();

        // Select a face plane (using first three vertices)
        Plane random_face_plane(
            tetrahedron1.vertex(0), 
            tetrahedron1.vertex(1), 
            tetrahedron1.vertex(2)
        );

        // Generate random point on the selected face
        Point vertex1 = GeometryUtils::generateRandomPointOnTriangle(
            tetrahedron1.vertex(0), tetrahedron1.vertex(1), tetrahedron1.vertex(2)
        );

        Point random_point_on_plane_1 = random_face_plane.projection(GeometryUtils::generateRandomPoint());
        

        // Compute face normal
        Vector normal = CGAL::normal(
            tetrahedron1.vertex(0), 
            tetrahedron1.vertex(1), 
            tetrahedron1.vertex(2)
        );


        // Ensure normal points outward
        Point fourth_vertex = tetrahedron1.vertex(3);
        Vector to_fourth = fourth_vertex - tetrahedron1.vertex(0);
        if (CGAL::to_double(normal * to_fourth) > 0) {
            normal = -normal;
        }

        // Setup coordinate system based on the normal
        GeometryUtils::CoordinateSystem coords(normal);

        auto attempt_start_time = std::chrono::steady_clock::now();
        bool success = false;

        // Spherical coordinate distributions
        std::uniform_real_distribution<> dist_theta(-(M_PI/2) + epsilon, (M_PI/2) - epsilon);
        std::uniform_real_distribution<> dist_phi(-(M_PI/2) + epsilon, (M_PI/2) - epsilon);

        // Attempt to generate a valid second tetrahedron
        while (std::chrono::steady_clock::now() - attempt_start_time < timeout) {
            bool valid_points = true;
            std::vector<Point> vertices;
            for(int i=0; i<2; i++){
                // Generate spherical coordinates
                double theta = dist_theta(gen);
                double phi = dist_phi(gen);

                // Calculate maximum allowable radius
                double r_max = coords.calculateMaxRadius(vertex1, theta, phi);

                // Generate radius within allowable range
                std::uniform_real_distribution<> dist_r(epsilon, r_max);
                double r = dist_r(gen);

                Point new_vertex = coords.sphericalToGlobal(vertex1, r, theta, phi);

                // Validate point is not inside original tetrahedron
                if (tetrahedron1.has_on_bounded_side(new_vertex)) {
                    valid_points = false;
                    continue;  // Try again if point is inside
                }

                vertices.push_back(new_vertex);

            }

            if (!valid_points) continue;

            // Construct potential second tetrahedron
            tetrahedron2 = Tetrahedron(
                vertex1, 
                random_point_on_plane_1,
                vertices[0],
                vertices[1] 
            );
            

            // Check tetrahedron validity and intersection
            if (!tetrahedron2.is_degenerate() && CGAL::do_intersect(tetrahedron1, tetrahedron2)) {
                success = true;
                break;
            }
        }

        // Return successful pair or continue searching
        if (success) {
            return std::make_pair(tetrahedron1, tetrahedron2);
        }
    }
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::PolygonIntersection() { // Polygon
    Tetrahedron tetrahedron1, tetrahedron2;
    auto start_time = std::chrono::steady_clock::now();
    constexpr std::chrono::seconds timeout(3);

    std::random_device rd;
    std::mt19937 gen(rd());

    while (true) {
        // Generate the first tetrahedron T1
        tetrahedron1 = GeometryUtils::generateRandomTetrahedron();

        // Select a face plane (using first three vertices)
        Plane random_face_plane(
            tetrahedron1.vertex(0), 
            tetrahedron1.vertex(1), 
            tetrahedron1.vertex(2)
        );

        // Project three random points onto the face plane
        Point random_point_on_plane_1 = random_face_plane.projection(GeometryUtils::generateRandomPoint());
        Point random_point_on_plane_2 = random_face_plane.projection(GeometryUtils::generateRandomPoint());
        Point random_point_on_plane_3 = random_face_plane.projection(GeometryUtils::generateRandomPoint());

        // Compute face normal
        Vector normal = CGAL::normal(
            tetrahedron1.vertex(0), 
            tetrahedron1.vertex(1), 
            tetrahedron1.vertex(2)
        );

        // Calculate centroid of projected points
        Point centroid = CGAL::centroid(
            random_point_on_plane_1, 
            random_point_on_plane_2, 
            random_point_on_plane_3
        );

        // Ensure normal points outward
        Point fourth_vertex = tetrahedron1.vertex(3);
        Vector to_fourth = fourth_vertex - tetrahedron1.vertex(0);
        if (CGAL::to_double(normal * to_fourth) > 0) {
            normal = -normal;
        }

        // Setup coordinate system based on the normal
        GeometryUtils::CoordinateSystem coords(normal);

        auto attempt_start_time = std::chrono::steady_clock::now();
        bool success = false;

        // Spherical coordinate distributions
        std::uniform_real_distribution<> dist_theta(-(M_PI/2) + epsilon, (M_PI/2) - epsilon);
        std::uniform_real_distribution<> dist_phi(-(M_PI/2) + epsilon, (M_PI/2) - epsilon);

        // Attempt to generate a valid second tetrahedron
        while (std::chrono::steady_clock::now() - attempt_start_time < timeout) {
            // Generate spherical coordinates
            double theta = dist_theta(gen);
            double phi = dist_phi(gen);

            // Calculate maximum allowable radius
            double r_max = coords.calculateMaxRadius(centroid, theta, phi);

            // Generate radius within allowable range
            std::uniform_real_distribution<> dist_r(epsilon, r_max);
            double r = dist_r(gen);

            // Generate fourth vertex in global space
            Point new_vertex = coords.sphericalToGlobal(centroid, r, theta, phi);

            // Validate point is not inside original tetrahedron
            if (tetrahedron1.has_on_bounded_side(new_vertex)) {
                continue;  // Try again if point is inside
            }

            // Construct potential second tetrahedron
            tetrahedron2 = Tetrahedron(
                random_point_on_plane_1, 
                random_point_on_plane_2,
                random_point_on_plane_3, 
                new_vertex
            );

            // Check tetrahedron validity and intersection
            if (!tetrahedron2.is_degenerate() && CGAL::do_intersect(tetrahedron1, tetrahedron2)) {
                success = true;
                break;
            }
        }

        // Return successful pair or continue searching
        if (success) {
            return std::make_pair(tetrahedron1, tetrahedron2);
        }
    }
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::PolyhedronIntersection() { // Polyhedron
    // Most intersecting tetrahedron pairs form polyhedron
    Tetrahedron tetrahedron1, tetrahedron2;

    do {

        tetrahedron1 = GeometryUtils::generateRandomTetrahedron();
        tetrahedron2 = GeometryUtils::generateRandomTetrahedron();

    } while (! CGAL::do_intersect(tetrahedron1, tetrahedron2));

    return std::make_pair(tetrahedron1, tetrahedron2);
}

