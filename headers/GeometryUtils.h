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
    static Point generateRandomPoint();
    static Point generateRandomPointOnTriangle(const Point& A, const Point& B, const Point& C);
    static Point generateRandomPointOutsideTetrahedron(const Tetrahedron tetrahedron);
    static Tetrahedron generateRandomTetrahedron();

    class CoordinateSystem {
    public:
        Vector x_axis;
        Vector y_axis;
        Vector z_axis;
        

        CoordinateSystem(const Vector& n);

        Point sphericalToGlobal(const Point& origin, double r, double theta, double phi) const;
        double calculateMaxRadius(const Point& origin, double theta, double phi) const;
    };

};

#endif // GEOMETRYUTILS_H

