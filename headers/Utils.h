// Utils.h
#pragma once
#include "Types.h"
#include "OBJWriter.h"

void post_processing(std::unique_ptr<BaseWriter>& writer, const std::string& format) {
    if (format == "obj") {
        OBJWriter* obj_writer = dynamic_cast<OBJWriter*>(writer.get());
        if (obj_writer != nullptr) {
            obj_writer->zipDirectory();
        }
    } if (format == "csv") {
        
    }
}

void print_progress_bar(int i, int NUMBER_OF_ENTRIES) {
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