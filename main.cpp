#include "headers/CSVWriter.h"
#include "headers/JSONWriter.h"
#include "headers/OBJWriter.h"
#include "headers/GeometryUtils.h"
#include "headers/TetrahedronFactory.h"
#include "headers/Types.h"
#include "headers/Utils.h"
#include "headers/Config.h"

int main() {
    try {
        srand(static_cast<unsigned int>(time(nullptr)));

        Configuration config;
        const int number_of_entries = config.getDatasetSize();
        const std::string format = config.getOutputFormat();
        const auto& distribution = config.getIntersectionDistribution();
        const auto precision = config.getPrecision();
        const auto min_volume = config.getMinVolume();
        const auto max_volume = config.getMaxVolume();
        const auto num_bins = config.getNumBins();

        auto writer = BaseWriter::createWriter(format, number_of_entries, precision);
        if (!writer) {
            std::cerr << "Failed to create writer." << std::endl;
            return 1;
        }

        // Calculate entries per type
        std::vector<int> entries_per_type(distribution.size());
        std::vector<int> generated_per_type(distribution.size(), 0);
        int remaining_entries = number_of_entries;

        for (size_t i = 0; i < distribution.size(); i++) {
            entries_per_type[i] = static_cast<int>((distribution[i] / 100.0) * number_of_entries);
            remaining_entries -= entries_per_type[i];
        }
        entries_per_type[0] += remaining_entries;
        std::unique_ptr<TetrahedronFactory> tetrahedron_factory = std::make_unique<TetrahedronFactory>();

        const auto size_of_interval = (max_volume - min_volume) / num_bins;
        std::vector<int> volume_distribution (num_bins, 0);
        const auto number_of_entries_per_bin = static_cast<int> (entries_per_type[4] / num_bins); // uniform distribution of volume

        // Generate tetrahedrons based on configuration
        for (int i = 0; i < number_of_entries; ++i) {
            std::unique_ptr<Tetrahedron> tetrahedron1;
            std::unique_ptr<Tetrahedron> tetrahedron2;

            // Find next type that needs more entries
            int type = 0;
            for (size_t j = 0; j < entries_per_type.size(); j++) {
                if (generated_per_type[j] < entries_per_type[j]) {
                    type = j + 1;
                    generated_per_type[j]++;
                    break;
                }
            }

            auto tetrahedron_pair = tetrahedron_factory->createRandomTetrahedronPair(type);
            tetrahedron1 = std::make_unique<Tetrahedron>(tetrahedron_pair.first);
            tetrahedron2 = std::make_unique<Tetrahedron>(tetrahedron_pair.second);

            bool intersection_status = GeometryUtils::checkIntersection(*tetrahedron1, *tetrahedron2);
            double intersection_volume = GeometryUtils::getIntersectionVolume(*tetrahedron1, *tetrahedron2);

            if(type == 5){

                if(intersection_volume < min_volume || intersection_volume > max_volume) {
                    // If volume is outside of range, discard entry
                    generated_per_type[type - 1]--;
                    i--;
                    continue;
                }

                // int bin = static_cast<int>(intersection_volume / size_of_interval) - 1;

                // if(bin < 0){
                //     bin = 0;
                // }else if(bin >= num_bins){
                //     bin = num_bins - 1;
                // }
                
                // assert(bin >= 0 && bin < num_bins);

                // if(volume_distribution[bin] >= number_of_entries_per_bin){
                //     // If bin is full, discard entry
                //     generated_per_type[type - 1]--;
                //     i--;
                //     continue;
                // }

                // volume_distribution[bin]++;
            }

            writer->writeEntry(*tetrahedron1, *tetrahedron2, intersection_volume, intersection_status);

            print_progress_bar(i + 1, number_of_entries);

        }
        writer.reset();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << std::endl;
    return 0;
}