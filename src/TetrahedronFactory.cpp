#include "TetrahedronFactory.h"
#include <CGAL/Random.h>
#include <CGAL/enum.h>
#include <chrono>
#include <cmath>
#include <random>
#include "GeometryUtils.h"

static CGAL::Random randomGenerator; // Static instance, initialized once

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
            return createType1TetrahedronPair(); // No intersection
        case 2:
            return createType2TetrahedronPair(); // Point
        case 3:
            return createType3TetrahedronPair(); // Line
        case 4:
            return createType4TetrahedronPair(); // Polygon
        case 5:
            return createType5TetrahedronPair(); // Polyhedron
        default:
            throw std::invalid_argument("Invalid tetrahedron type: " + std::to_string(type));
    }
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType1TetrahedronPair() { // No intersection

    Tetrahedron tetrahedron1, tetrahedron2;

    do {
        tetrahedron1 = GeometryUtils::generateRandomTetrahedron();
        tetrahedron2 = GeometryUtils::generateRandomTetrahedron();

    } while (CGAL::do_intersect(tetrahedron1, tetrahedron2));

    return std::make_pair(tetrahedron1, tetrahedron2);
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType2TetrahedronPair() {
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
        GeometryUtils::CoordinateSystem coords(normal);

        auto attempt_start_time = std::chrono::steady_clock::now();
        bool success = false;

        // Distribution for spherical coordinates
        std::uniform_real_distribution<> dist_theta(epsilon, M_PI - epsilon);
        std::uniform_real_distribution<> dist_phi(epsilon, M_PI - epsilon);

        while (std::chrono::steady_clock::now() - attempt_start_time < timeout) {
            std::vector<Point> new_vertices = {vertex1};
            bool valid_points = true;

            // Generate three more vertices for T2
            for (int i = 0; i < 3; ++i) {
                double theta = dist_theta(gen);
                double phi = dist_phi(gen);
                
                // Calculate maximum allowable radius for this direction
                double r_max = coords.calculateMaxRadius(vertex1, theta, phi);
                if (r_max < epsilon) {
                    valid_points = false;
                    break;
                }

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

            // Create and validate T2
            tetrahedron2 = Tetrahedron(
                new_vertices[0], new_vertices[1], 
                new_vertices[2], new_vertices[3]
            );

            if (!tetrahedron2.is_degenerate() && CGAL::do_intersect(tetrahedron1, tetrahedron2)) {
                success = true;
                break;
            }
        }

        if (success) {
            return std::make_pair(tetrahedron1, tetrahedron2);
        }
    }
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType3TetrahedronPair() { // Line

}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType4TetrahedronPair() { // Polygon

}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType5TetrahedronPair() { // Polyhedron
    // Most intersecting tetrahedron pairs form polyhedron
    Tetrahedron tetrahedron1, tetrahedron2;

    do {

        tetrahedron1 = GeometryUtils::generateRandomTetrahedron();
        tetrahedron2 = GeometryUtils::generateRandomTetrahedron();

    } while (! CGAL::do_intersect(tetrahedron1, tetrahedron2));

    return std::make_pair(tetrahedron1, tetrahedron2);
}

