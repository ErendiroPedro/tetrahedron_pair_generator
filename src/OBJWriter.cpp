#include "OBJWriter.h"
#include "Types.h"

int OBJWriter::fileCounter = 0;

void OBJWriter::writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, double volume, bool intersectionStatus) {
    std::stringstream ss;
    ss << "tetrahedron_pair_" << std::setw(5) << std::setfill('0') << ++fileCounter << ".obj";
    std::ofstream outFile(baseDir + "/" + ss.str());
    if (!outFile) {
        throw std::runtime_error("Unable to open file: " + baseDir + ss.str());
    }

    outFile << "# Tetrahedrons pair\n";
    int vertexOffset = 1;

    outFile << "# Tetrahedron 1\n";
    writeTetrahedron(outFile, T1, vertexOffset);
    vertexOffset += 4;

    outFile << "# Tetrahedron 2\n";
    writeTetrahedron(outFile, T2, vertexOffset);

    outFile << "# Intersection status: " << (intersectionStatus ? "1" : "0") << "\n";
    outFile.close();
}

void OBJWriter::writeTetrahedron(std::ofstream& outFile, const Tetrahedron& tetrahedron, int& vertexOffset) {
    for (int i = 0; i < 4; ++i) {
        Point p = tetrahedron.vertex(i);
        outFile << "v " << CGAL::to_double(p.x()) << " " << CGAL::to_double(p.y()) << " " << CGAL::to_double(p.z()) << "\n";
    }

    std::vector<std::vector<int>> faces = {{0, 1, 2}, {0, 2, 3}, {0, 3, 1}, {1, 2, 3}};
    for (const auto& face : faces) {
        outFile << "f ";
        for (int vertex : face) {
            outFile << (vertex + vertexOffset) << " ";
        }
        outFile << "\n";
    }
}

void OBJWriter::zipDirectory() {
    std::string command = "zip -r " + baseDir + ".zip " + baseDir;
    int result = system(command.c_str());

    if (result != 0) {
        throw std::runtime_error("Failed to zip directory");
    }

    command = "rm -rf " + baseDir;
    result = system(command.c_str());

    if (result != 0) {
        throw std::runtime_error("Failed to delete directory");
    }
}