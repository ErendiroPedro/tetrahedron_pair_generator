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
        writer->writeEntry(*tetrahedron1, *tetrahedron2, intersection_status);
    }

    if (format == "obj") {
        OBJWriter* obj_writer = dynamic_cast<OBJWriter*>(writer.get());
        if (obj_writer != nullptr) {
            obj_writer->zipDirectory();
        }
    }

    return 0;
}