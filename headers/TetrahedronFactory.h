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
    
};

#endif // TETRAHEDRONFACTORY_H
