#include "BaseWriter.h"
#include "CSVWriter.h"
#include "JSONWriter.h"
#include "OBJWriter.h"

std::string formatFilename(const std::string& extension, int number_of_entries){
    std::string base_filename = "../output/tetrahedron_pair_";
    if (number_of_entries < 1000) {
        return base_filename + std::to_string(number_of_entries) + "_dataset." + extension;
    } else {
        int exponent = static_cast<int>(std::log10(number_of_entries));
        char suffix = 'k';
        int divisor = 1000;
        if (exponent >= 6) {
            suffix = 'M';
            divisor = 1000000;
        }

        double formatted_number = number_of_entries / static_cast<double>(divisor);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << formatted_number;
        std::string formatted_number_str = ss.str();

        // Remove trailing zero after decimal point
        if (formatted_number_str.back() == '0') {
            formatted_number_str.pop_back();
            formatted_number_str.pop_back(); // remove the decimal point as well
        }

        return base_filename + formatted_number_str + suffix + "_dataset." + extension;
    }
}

std::unique_ptr<BaseWriter> BaseWriter::createWriter(const std::string& type, int numberOfEntries, int prec) {
    std::string filename = formatFilename(type, numberOfEntries);
    if (type == "json") {
        return std::make_unique<JSONWriter>(filename);
    } else if(type == "csv") {
        return std::make_unique<CSVWriter>(filename, prec);
    } else if(type == "obj"){
        std::string directory = filename.substr(0, filename.find_last_of(".")); // Remove extension
        mkdir(directory.c_str(), 0777); // Create directory with read/write permissions
        return std::make_unique<OBJWriter>(directory);
    }
    return nullptr;
}

