#ifndef GEOMETRYUTILS_H
#define GEOMETRYUTILS_H

#include "Types.h"

class GeometryUtils {
public:
    static bool checkIntersection(const Tetrahedron& T1, const Tetrahedron& T2);
    static IntersectionType getIntersectionClassification(const Tetrahedron& T1, const Tetrahedron& T2); 
    static std::vector<Point> getIntersectionShape(const Tetrahedron& T1, const Tetrahedron& T2);
    static double getIntersectionVolume(const Tetrahedron& T1, const Tetrahedron& T2);
    static Mesh tetrahedronToMesh(const Tetrahedron& T);
};

#endif // GEOMETRYUTILS_H

