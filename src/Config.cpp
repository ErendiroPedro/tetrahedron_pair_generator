#include "Config.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

Configuration::Configuration(const std::string& config_path) {
    loadConfig(config_path);
    validateConfig();
}

void Configuration::loadConfig(const std::string& config_path) {
    std::ifstream config_file(config_path);
    if (!config_file.is_open()) {
        throw std::runtime_error("Unable to open config file: " + config_path);
    }

    nlohmann::json j;
    config_file >> j;

    output_format = j["output_format"]["value"].get<std::string>();
    precision = j["precision"]["value"].get<int>();
    dataset_size = j["dataset_size"]["value"].get<int>();
    intersection_distribution = j["intersection_distribution"]["value"].get<std::vector<double>>();
    volume_min = j["volume_range"]["value"]["min"].get<double>();
    volume_max = j["volume_range"]["value"]["max"].get<double>();
    num_bins = j["num_bins"]["value"].get<int>();
}

void Configuration::validateConfig() {
    if (precision < 1 || precision > 16) {
        throw std::invalid_argument("Precision must be between 1 and 16");
    }

    if (dataset_size <= 0) {
        throw std::invalid_argument("Dataset size must be greater than 0");
    }

    if (intersection_distribution.size() != 5) {
        throw std::invalid_argument("Intersection distribution must have exactly 5 values");
    }

    if (volume_min < 0 || volume_max > 0.333333333333333 || volume_min >= volume_max) {
        throw std::invalid_argument("Invalid volume range");
    }

    if (num_bins <= 0) {
        throw std::invalid_argument("Number of bins must be greater than 0");
    }

    double sum = 0;
    for (double d : intersection_distribution) {
        sum += d;
    }

    // Allow for small deviations from 100
    const double tolerance = 1e-6;
    const double maxAllowedDeviation = 0.5; // Allow up to 0.5% deviation

    if (std::abs(sum - 100.0) > tolerance) {
        if (std::abs(sum - 100.0) <= maxAllowedDeviation) {
            // Calculate the difference and adjust first value
            double difference = 100.0 - sum;
            intersection_distribution[0] += difference;

            // Log the adjustment
            std::ofstream logFile("log/generator.log", std::ios::app);
            std::stringstream logMessage;
            logMessage << "Warning: Distribution sum was " << sum 
                      << ". Adjusted first intersection type by " 
                      << difference << " to achieve sum of 100." << std::endl;
            
            std::cout << logMessage.str();
            logFile << logMessage.str();
            logFile.close();
        } else {
            throw std::invalid_argument("Intersection distribution deviation too large. Must be within 1\% of 100");
        }
    }
}