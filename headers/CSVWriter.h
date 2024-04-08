#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "Types.h"
#include "BaseWriter.h"

class CSVWriter : public BaseWriter {
public:
    CSVWriter(const std::string& filename);
    ~CSVWriter();
    void writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, bool intersects);
private:
    std::ofstream outFile;
    std::vector<std::string> headers;
    void initializeHeaders();
    void writeHeaders();
    void writeVertex(const Point& p);
};

#endif // CSVWRITER_H
