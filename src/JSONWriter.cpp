#include <JSONWriter.h>

using json = nlohmann::json;

std::string JSONWriter::formatFilename(int number_of_entries) {
    std::string base_filename = "../output/tetrahedron_pair_";
    if (number_of_entries < 1000) {
        return base_filename + std::to_string(number_of_entries) + "_dataset.json";
    } else {
        int exponent = static_cast<int>(std::log10(number_of_entries));
        char suffix = 'k'; // default for thousands
        int divisor = 1000; // default for thousands
        if (exponent >= 6) {
            suffix = 'M'; // millions
            divisor = 1000000;
        }
        // For larger magnitudes, more cases can be added here
        double formatted_number = number_of_entries / static_cast<double>(divisor);
        // Format the number to one decimal place if not an integer
        std::string formatted_number_str = std::to_string(formatted_number);
        size_t dot_pos = formatted_number_str.find('.');
        if (dot_pos != std::string::npos && dot_pos + 2 < formatted_number_str.length()) {
            formatted_number_str = formatted_number_str.substr(0, dot_pos + 2);
        }
        return base_filename + formatted_number_str + suffix + "_dataset.json";
    }
}

JSONWriter::JSONWriter(const std::string& filename) {
    outFile.open(filename);
    if (!outFile) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    // Initialize your JSON object here if necessary
}

JSONWriter::~JSONWriter() {
    outFile << data.dump(4); // Pretty print with 4 spaces indent
    outFile.close();
}

void JSONWriter::writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, bool intersectionStatus) {
    json entry;
    entry["Id"] = idCounter++;
    entry["tetra1"] = tetrahedronToJson(T1);
    entry["tetra2"] = tetrahedronToJson(T2);
    entry["intersection_status"] = intersectionStatus ? 1 : 0;

    data.push_back(entry);
}

json JSONWriter::tetrahedronToJson(const Tetrahedron& tetrahedron) {
    json tetraJson;
    std::vector<std::vector<double>> vertices;
    std::vector<std::vector<int>> edges = {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};

    for (int i = 0; i < 4; ++i) {
        Point p = tetrahedron.vertex(i);
        std::vector<double> vertex = {
            static_cast<double>(CGAL::to_double(p.x())), 
            static_cast<double>(CGAL::to_double(p.y())), 
            static_cast<double>(CGAL::to_double(p.z()))
        };
        vertices.push_back(vertex);
    }

    tetraJson["vertices"] = vertices;
    tetraJson["edges"] = edges;

    return tetraJson;
}

