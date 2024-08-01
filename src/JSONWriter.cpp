#include <JSONWriter.h>

using json = nlohmann::json;

JSONWriter::JSONWriter(const std::string& filename){
    outFile.open(filename);
    if (!outFile) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
}

JSONWriter::~JSONWriter() {
    outFile << data.dump(4); // Pretty print with 4 spaces indent
    outFile.close();
}

void JSONWriter::writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, double volume, bool intersectionStatus) {
    json entry;
    entry["id"] = idCounter++;
    entry["tetrahedron_1"] = tetrahedronToJson(T1);
    entry["tetrahedron_2"] = tetrahedronToJson(T2);
    entry["intersection_status"] = intersectionStatus ? 1 : 0;

    data.push_back(entry);
}

json JSONWriter::tetrahedronToJson(const Tetrahedron& tetrahedron) {
    json tetraJson;
    std::vector<std::vector<double>> vertices;
    std::vector<std::vector<int>> edges = {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};

    for (int i = 0; i < 4; ++i) {
        Point p = tetrahedron.vertex(i);
        std::vector<double> vertex;

        std::ostringstream ss;
        
        ss << std::fixed << std::setprecision(precision) << CGAL::to_double(p.x());
        vertex.push_back(std::stod(ss.str()));
        ss.str("");  // Clear the stringstream

        ss << std::fixed << std::setprecision(precision) << CGAL::to_double(p.y());
        vertex.push_back(std::stod(ss.str()));
        ss.str("");

        ss << std::fixed << std::setprecision(precision) << CGAL::to_double(p.z());
        vertex.push_back(std::stod(ss.str()));

        vertices.push_back(vertex);
    }

    tetraJson["vertices"] = vertices;
    tetraJson["edges"] = edges;

    return tetraJson;
}

