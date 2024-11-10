#pragma once
#include "Types.h"


class Configuration {
public:
    Configuration(const std::string& config_path = "../config/config.json");
    
    std::string getOutputFormat() const { return output_format; }
    int getPrecision() const { return precision; }
    int getDatasetSize() const { return dataset_size; }
    const std::vector<double>& getIntersectionDistribution() const { return intersection_distribution; }
    double getMinVolume() const { return volume_min; }
    double getMaxVolume() const { return volume_max; }
    int getNumBins() const { return num_bins; }

private:
    void loadConfig(const std::string& config_path);
    void validateConfig();

    std::string output_format;
    int precision;
    int dataset_size;
    std::vector<double> intersection_distribution;
    double volume_min;
    double volume_max;
    int num_bins;
};