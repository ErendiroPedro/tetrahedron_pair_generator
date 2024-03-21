#ifndef TETRAHEDRONFACTORY_H
#define TETRAHEDRONFACTORY_H

#include "Types.h"

class TetrahedronFactory {
public:
    static std::pair<Tetrahedron, Tetrahedron> createRandomTetrahedronPair();
    // static std::pair<Tetrahedron, Tetrahedron> createType1TetrahedronPair();
    
private:

    static Point generateRandomPoint();
    static Point generateRandomPointOnTriangle(const Point& A, const Point& B, const Point& C);
};

#endif // TETRAHEDRONFACTORY_H
