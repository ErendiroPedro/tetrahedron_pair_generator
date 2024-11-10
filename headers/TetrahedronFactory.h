#ifndef TETRAHEDRONFACTORY_H
#define TETRAHEDRONFACTORY_H

#include "Types.h"

class TetrahedronFactory {
public:
    static std::pair<Tetrahedron, Tetrahedron> createRandomTetrahedronPair(int type = 0);
    static std::pair<Tetrahedron, Tetrahedron> createRandomTetrahedronPair();
    static std::pair<Tetrahedron, Tetrahedron> createType1TetrahedronPair();
    static std::pair<Tetrahedron, Tetrahedron> createType2TetrahedronPair();
    static std::pair<Tetrahedron, Tetrahedron> createType3TetrahedronPair();
    static std::pair<Tetrahedron, Tetrahedron> createType4TetrahedronPair();
    static std::pair<Tetrahedron, Tetrahedron> createType5TetrahedronPair();
    
private:

    static Point generateRandomPoint();
    static Point generateRandomPointOnTriangle(const Point& A, const Point& B, const Point& C);
};

#endif // TETRAHEDRONFACTORY_H
