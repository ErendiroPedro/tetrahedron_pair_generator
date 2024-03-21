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

std::string CSVWriter::formatFilename(int number_of_entries) {
    std::string base_filename = "../output/tetrahedron_pair_";
    if (number_of_entries < 1000) {
        return base_filename + std::to_string(number_of_entries) + "_dataset.csv";
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
        return base_filename + formatted_number_str + suffix + "_dataset.csv";
    }
}

void CSVWriter::writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, IntersectionType intersection_class, std::vector<Point> resulting_shape, bool intersects) {
    for (int i = 0; i < 4; ++i) {
        writeVertex(T1.vertex(i));
        outFile << ",";
    }

    for (int i = 0; i < 4; ++i) {
        writeVertex(T2.vertex(i));
        outFile << ",";
    }

    outFile << static_cast<int>(intersection_class) << ",";

    // Assuming MAX_VERTICES now refers to the maximum number of points allowed in resulting_shape
    int padVertices = MAX_VERTICES - resulting_shape.size(); // MAX_VERTICES now indicates the max number of vertices allowed in the resulting shape.
    outFile << "\""; // Start of the parenthesis encapsulated string for the resulting shape
    for (const auto& vertex : resulting_shape) {
        outFile << "(";
        writeVertex(vertex);
        outFile << ")";
        if (&vertex != &resulting_shape.back() || padVertices > 0) { // Check if it's not the last vertex or if padding is needed
            outFile << ",";
        }
    }

    // Pad the remaining vertices with -1 as the placeholder for each coordinate
    for (int i = 0; i < padVertices; ++i) {
        outFile << "(-1,-1,-1)"; // Adjusted to follow the new format
        if (i < padVertices - 1) { // Only add a comma if it's not the last placeholder group
            outFile << ",";
        }
    }
    outFile << "\","; // End of the parenthesis encapsulated string for the resulting shape and adding comma to separate next field

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

    headers.push_back("intersection_class");
    headers.push_back("intersection_status");
}

void CSVWriter::writeHeaders() {
    for (size_t i = 0; i < headers.size(); ++i) {
        outFile << headers[i];
        if (i < headers.size() - 1) outFile << ",";
    }
    outFile << "\n";
}

void CSVWriter::writeVertex(const Point& p) {
    outFile << p.x() << "," << p.y() << "," << p.z();
}
