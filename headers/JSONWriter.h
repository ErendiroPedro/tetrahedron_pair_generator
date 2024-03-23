#ifndef JSONWRITER_H
#define JSONWRITER_H

#include "Types.h" // Include the Tetrahedron class definition

class JSONWriter {
public:
    JSONWriter(const std::string& filename);
    ~JSONWriter();
    static std::string formatFilename(int number_of_entries);
    void writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, bool intersectionStatus);

private:
    std::ofstream outFile;
    json data;
    int idCounter = 1;

    json tetrahedronToJson(const Tetrahedron& tetrahedron);
};

#endif // JSONWRITER_H