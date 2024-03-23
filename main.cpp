#include "headers/CSVWriter.h"
#include "headers/JSONWriter.h"
#include "headers/GeometryUtils.h"
#include "headers/TetrahedronFactory.h"
#include "headers/Types.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <NUMBER_OF_ENTRIES>" << std::endl;
        return 1;
    }

    int NUMBER_OF_ENTRIES = std::atoi(argv[1]);
    if(NUMBER_OF_ENTRIES <= 0) {
        std::cerr << "The number of entries must be a positive integer." << std::endl;
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr))); // Seed randomness

    std::string filename = JSONWriter::formatFilename(NUMBER_OF_ENTRIES);
    JSONWriter writer(filename);

    std::unique_ptr<TetrahedronFactory> tetrahedron_factory = std::make_unique<TetrahedronFactory>();
    for (int i = 0; i < NUMBER_OF_ENTRIES; ++i) {
        auto tetrahedronPair = tetrahedron_factory->createRandomTetrahedronPair();
        std::unique_ptr<Tetrahedron> tetrahedron1 = std::make_unique<Tetrahedron>(tetrahedronPair.first);
        std::unique_ptr<Tetrahedron> tetrahedron2 = std::make_unique<Tetrahedron>(tetrahedronPair.second);
        
        bool intersection_status = GeometryUtils::checkIntersection(*tetrahedron1, *tetrahedron2);
        // IntersectionType intersection_class = GeometryUtils::getIntersectionClassification(*tetrahedron1, *tetrahedron2);
        // std::vector<Point> resulting_shape = GeometryUtils::getIntersectionShape(*tetrahedron1, *tetrahedron2);
        writer.writeEntry(*tetrahedron1, *tetrahedron2, intersection_status);
    }

    return 0;
}

