#pragma once

#include "Types.h"

std::string formatFilename(const std::string& extension, int numberOfEntries);

class BaseWriter {
public:
    static std::unique_ptr<BaseWriter> createWriter(const std::string& type, int numberOfEntries);
    BaseWriter() = default;
    virtual ~BaseWriter() = default;
    virtual void writeEntry(const Tetrahedron& tetrahedron1, const Tetrahedron& tetrahedron2, bool intersection_status) = 0;
protected:
    int precision = 6;
};
