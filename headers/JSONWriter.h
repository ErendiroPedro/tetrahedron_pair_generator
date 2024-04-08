#pragma once

#include "Types.h"
#include "BaseWriter.h"

class JSONWriter : public BaseWriter {
public:
    JSONWriter(const std::string& filename);
    ~JSONWriter();
    void writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, bool intersectionStatus);

private:
    std::ofstream outFile;
    json data;
    int idCounter = 1;

    json tetrahedronToJson(const Tetrahedron& tetrahedron);
};