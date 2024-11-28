#ifndef TETRAHEDRONFACTORY_H
#define TETRAHEDRONFACTORY_H

#include "Types.h"

class TetrahedronFactory {
public:
    static std::pair<Tetrahedron, Tetrahedron> createRandomTetrahedronPair(int type = 0);
    static std::pair<Tetrahedron, Tetrahedron> createRandomTetrahedronPair();
    static std::pair<Tetrahedron, Tetrahedron> NoIntersection();
    static std::pair<Tetrahedron, Tetrahedron> PointIntersection();
    static std::pair<Tetrahedron, Tetrahedron> LineIntersection();
    static std::pair<Tetrahedron, Tetrahedron> PolygonIntersection();
    static std::pair<Tetrahedron, Tetrahedron> PolyhedronIntersection();
    
};

#endif // TETRAHEDRONFACTORY_H
