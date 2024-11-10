#include "TetrahedronFactory.h"
#include <CGAL/Random.h>
#include <CGAL/enum.h>

static CGAL::Random randomGenerator; // Static instance, initialized once

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createRandomTetrahedronPair() {

    Point vertexA, vertexB, vertexC, vertexD;
    Point vertex0, vertex1, vertex2, vertex3;
    Tetrahedron tetrahedron1, tetrahedron2;

    do {

        vertexA = generateRandomPoint();
        vertexB = generateRandomPoint();
        vertexC = generateRandomPoint();
        vertexD = generateRandomPoint();

        vertex0 = generateRandomPoint();
        vertex1 = generateRandomPoint();
        vertex2= generateRandomPoint();
        vertex3 = generateRandomPoint();

        tetrahedron1 = Tetrahedron(vertexA, vertexB, vertexC, vertexD);
        tetrahedron2 = Tetrahedron(vertex0, vertex1, vertex2, vertex3);


    } while (tetrahedron1.is_degenerate() || tetrahedron2.is_degenerate());

    return std::make_pair(tetrahedron1, tetrahedron2);
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createRandomTetrahedronPair(int type) {
    // If type is 0, randomly select a type between 1-5
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

    Point vertexA, vertexB, vertexC, vertexD;
    Point vertex0, vertex1, vertex2, vertex3;
    Tetrahedron tetrahedron1, tetrahedron2;

    do {

        vertexA = generateRandomPoint();
        vertexB = generateRandomPoint();
        vertexC = generateRandomPoint();
        vertexD = generateRandomPoint();

        vertex0 = generateRandomPoint();
        vertex1 = generateRandomPoint();
        vertex2= generateRandomPoint();
        vertex3 = generateRandomPoint();

        tetrahedron1 = Tetrahedron(vertexA, vertexB, vertexC, vertexD);
        tetrahedron2 = Tetrahedron(vertex0, vertex1, vertex2, vertex3);


    } while (tetrahedron1.is_degenerate() || tetrahedron2.is_degenerate() || CGAL::do_intersect(tetrahedron1, tetrahedron2));

    return std::make_pair(tetrahedron1, tetrahedron2);
}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType2TetrahedronPair() { // Point

}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType3TetrahedronPair() { // Line

}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType4TetrahedronPair() { // Polygon

}

std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType5TetrahedronPair() { // Polyhedron

    Point vertexA, vertexB, vertexC, vertexD;
    Point vertex0, vertex1, vertex2, vertex3;
    Tetrahedron tetrahedron1, tetrahedron2;

    do {

        vertexA = generateRandomPoint();
        vertexB = generateRandomPoint();
        vertexC = generateRandomPoint();
        vertexD = generateRandomPoint();

        vertex0 = generateRandomPoint();
        vertex1 = generateRandomPoint();
        vertex2= generateRandomPoint();
        vertex3 = generateRandomPoint();

        tetrahedron1 = Tetrahedron(vertexA, vertexB, vertexC, vertexD);
        tetrahedron2 = Tetrahedron(vertex0, vertex1, vertex2, vertex3);


    } while (tetrahedron1.is_degenerate() || tetrahedron2.is_degenerate() || ! CGAL::do_intersect(tetrahedron1, tetrahedron2));

    return std::make_pair(tetrahedron1, tetrahedron2);
}


// std::pair<Tetrahedron, Tetrahedron> TetrahedronFactory::createType2TetrahedronPair() {
    
//     Point vertexA, vertexB, vertexC, vertexD;
//     Tetrahedron tetrahedron1, tetrahedron2;

//     do {
//         vertexA = generateRandomPoint();
//         vertexB = generateRandomPoint();
//         vertexC = generateRandomPoint();
//         vertexD = generateRandomPoint();

//         tetrahedron1 = Tetrahedron(vertexA, vertexB, vertexC, vertexD);
//     } while (tetrahedron1.is_degenerate());


//     Point vertex1, vertex2, vertex3, vertex4;

//     vertex1 = generateRandomPointOnTriangle(tetrahedron1.vertex(0), tetrahedron1.vertex(1), tetrahedron1.vertex(2));

//     // In development
             

//     return std::make_pair(tetrahedron1, tetrahedron2);
// }

Point TetrahedronFactory::generateRandomPoint() {
    double x = randomGenerator.get_double(0.0, 1.0);
    double y = randomGenerator.get_double(0.0, 1.0);
    double z = randomGenerator.get_double(0.0, 1.0);
    return Point(x, y, z);
}

Point TetrahedronFactory::generateRandomPointOnTriangle(const Point& A, const Point& B, const Point& C) {
    CGAL::Random_points_in_triangle_3<Point> gen(A, B, C);
    return *gen++;
}