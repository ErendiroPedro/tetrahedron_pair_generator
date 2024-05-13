#include "CSVWriter.h"

unsigned int MAX_VERTICES = 16;

CSVWriter::CSVWriter(const std::string& filename) {
    outFile.open(filename);
    if (!outFile) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    initializeHeaders();
    writeHeaders();
}

CSVWriter::~CSVWriter() {
    outFile.close();
}

void CSVWriter::writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, double volume, bool intersects) {
    for (int i = 0; i < 4; ++i) {
        writeVertex(T1.vertex(i));
        outFile << ",";
    }

    for (int i = 0; i < 4; ++i) {
        writeVertex(T2.vertex(i));
        outFile << ",";
    }

    // outFile << static_cast<int>(intersection_class) << ",";

    // Assuming MAX_VERTICES now refers to the maximum number of points allowed in resulting_shape
    // int padVertices = MAX_VERTICES - resulting_shape.size(); // MAX_VERTICES now indicates the max number of vertices allowed in the resulting shape.
    // outFile << "\""; // Start of the parenthesis encapsulated string for the resulting shape
    // for (const auto& vertex : resulting_shape) {
    //     outFile << "(";
    //     writeVertex(vertex);
    //     outFile << ")";
    //     if (&vertex != &resulting_shape.back() || padVertices > 0) { // Check if it's not the last vertex or if padding is needed
    //         outFile << ",";
    //     }
    // }

    // Pad the remaining vertices with -1 as the placeholder for each coordinate
    // for (int i = 0; i < padVertices; ++i) {
    //     outFile << "(-1,-1,-1)"; // Adjusted to follow the new format
    //     if (i < padVertices - 1) { // Only add a comma if it's not the last placeholder group
    //         outFile << ",";
    //     }
    // }
    // outFile << "\","; // End of the parenthesis encapsulated string for the resulting shape and adding comma to separate next field
    outFile << std::fixed << std::setprecision(precision) << volume << ",";
    outFile << (intersects ? 1 : 0);
    outFile << "\n";
}

void CSVWriter::initializeHeaders() {
    for (int i = 1; i <= 2; ++i) {
        for (int v = 1; v <= 4; ++v) {
            headers.push_back("T" + std::to_string(i) + "_v" + std::to_string(v) + "_x");
            headers.push_back("T" + std::to_string(i) + "_v" + std::to_string(v) + "_y");
            headers.push_back("T" + std::to_string(i) + "_v" + std::to_string(v) + "_z");
        }
    }

    // headers.push_back("intersection_class");
    headers.push_back("intersection_volume");
    headers.push_back("intersection_status");
}

void CSVWriter::writeHeaders() {
    for (size_t i = 0; i < headers.size(); ++i) {
        outFile << headers[i];
        if (i < headers.size() - 1) outFile << ",";
    }
    outFile << "\n";
}

void CSVWriter::writeVertex(const Point& vertex) {
    std::ostringstream strs;
    strs << std::fixed << std::setprecision(precision) << static_cast<double>(CGAL::to_double(vertex.x()));
    outFile << strs.str() << ",";
    strs.str(""); strs.clear(); // Clear the stringstream for the next coordinate
    strs << std::fixed << std::setprecision(precision) << static_cast<double>(CGAL::to_double(vertex.y()));
    outFile << strs.str() << ",";
    strs.str(""); strs.clear(); // Clear the stringstream for the next coordinate
    strs << std::fixed << std::setprecision(precision) << static_cast<double>(CGAL::to_double(vertex.z()));
    outFile << strs.str();
}
