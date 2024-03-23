// Types.h
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <variant>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <utility>
#include <iostream>
#include <cmath>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Tetrahedron_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/point_generators_3.h>
#include <nlohmann/json.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel ExactKernel;

typedef CGAL::Plane_3<ExactKernel> Plane;
typedef CGAL::Vector_3<ExactKernel> Vector;
typedef CGAL::Point_3<ExactKernel> Point;
typedef CGAL::Segment_3<ExactKernel> Segment;
typedef std::vector<CGAL::Point_3<ExactKernel>> Polygon;
typedef CGAL::Triangle_3<ExactKernel> Triangle;
typedef CGAL::Tetrahedron_3<ExactKernel> Tetrahedron;
typedef CGAL::Polyhedron_3<ExactKernel> Polyhedron;
typedef CGAL::Nef_polyhedron_3<ExactKernel> Nef_polyhedron;

typedef CGAL::Random_points_in_triangle_3<ExactKernel::Point_3> PointGeneratorInTriangle;


typedef CGAL::Surface_mesh<ExactKernel::Point_3> Mesh;
typedef Mesh::Vertex_index vertex_descriptor;
typedef Mesh::Face_index face_descriptor;

typedef std::variant<Point, Segment, Polygon, Polyhedron> ResultingVariant;
typedef std::optional<ResultingVariant> ResultingShape;


enum class IntersectionType {
    None = 0,
    Point,
    Segment,
    Polygon,
    Polyhedron
};

using json = nlohmann::json;