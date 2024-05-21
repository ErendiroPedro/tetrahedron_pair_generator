#include "headers/CSVWriter.h"
#include "headers/JSONWriter.h"
#include "headers/OBJWriter.h"
#include "headers/GeometryUtils.h"
#include "headers/TetrahedronFactory.h"
#include "headers/Types.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <NUMBER_OF_ENTRIES> <FORMAT>" << std::endl;
        return 1;
    }

    int NUMBER_OF_ENTRIES = std::atoi(argv[1]);
    std::string format = argv[2];

    if(NUMBER_OF_ENTRIES <= 0) {
        std::cerr << "The number of entries must be a positive integer." << std::endl;
        return 1;
    }

    if (format != "json" && format != "csv" && format != "obj") {
        std::cerr << "Unsupported format. Please choose 'json' or 'csv'." << std::endl;
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr))); // Seed randomness

    auto writer = BaseWriter::createWriter(format, NUMBER_OF_ENTRIES);
    if (!writer) {
        std::cerr << "Failed to create writer." << std::endl;
        return 1;
    }

    std::unique_ptr<TetrahedronFactory> tetrahedron_factory = std::make_unique<TetrahedronFactory>();
    for (int i = 0; i < NUMBER_OF_ENTRIES; ++i) {
        std::unique_ptr<Tetrahedron> tetrahedron1;
        std::unique_ptr<Tetrahedron> tetrahedron2;

        if (i % 2 == 0) {
            auto tetrahedronPair = tetrahedron_factory->createType1TetrahedronPair();
            tetrahedron1 = std::make_unique<Tetrahedron>(tetrahedronPair.first);
            tetrahedron2 = std::make_unique<Tetrahedron>(tetrahedronPair.second);
        } else {
            auto tetrahedronPair = tetrahedron_factory->createType4TetrahedronPair();
            tetrahedron1 = std::make_unique<Tetrahedron>(tetrahedronPair.first);
            tetrahedron2 = std::make_unique<Tetrahedron>(tetrahedronPair.second);
        }

        bool intersection_status = GeometryUtils::checkIntersection(*tetrahedron1, *tetrahedron2);
        double intersection_volume = GeometryUtils::getIntersectionVolume(*tetrahedron1, *tetrahedron2);
        writer->writeEntry(*tetrahedron1, *tetrahedron2, intersection_volume, intersection_status);

        // Print progress bar
        std::cout << "\rProgress: [";
        int pos = static_cast<int>(static_cast<float>(i) / NUMBER_OF_ENTRIES * 50);
        for (int j = 0; j < 50; ++j) {
            if (j < pos) std::cout << "=";
            else if (j == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(static_cast<float>(i) / NUMBER_OF_ENTRIES * 100.0) << " %";
        std::cout.flush();

        }

    if (format == "obj") {
        OBJWriter* obj_writer = dynamic_cast<OBJWriter*>(writer.get());
        if (obj_writer != nullptr) {
            obj_writer->zipDirectory();
        }
    } if (format == "csv") {
    //     std::string filename = "your_file.csv"; // Replace with your filename
    //     std::ifstream inFile(filename);
    //     std::vector<std::tuple<double, std::string>> data;

    //     std::string line;
    //     while (std::getline(inFile, line)) {
    //         std::istringstream iss(line);
    //         std::string tetrahedron1, tetrahedron2, intersectionStatus;
    //         if (!(iss >> tetrahedron1 >> tetrahedron2 >> intersectionStatus)) { break; } // error

    //         // Parse the x coordinate of the first vertex of the first tetrahedron
    //         // This assumes that the tetrahedron is represented as a string "x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4"
    //         std::string::size_type sz;
    //         double x = std::stod(tetrahedron1, &sz);
    //         data.push_back(std::make_tuple(x, line));
    //     }

    //     // Sort the data
    //     std::sort(data.begin(), data.end());

    //     // Write the sorted data back out to the CSV file
    //     std::ofstream outFile(filename);
    //     for (const auto& row : data) {
    //         outFile << std::get<1>(row) << "\n";
    // }
    }

    std::cout << std::endl;
    
    return 0;
}