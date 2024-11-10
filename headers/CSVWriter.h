#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "Types.h"
#include "BaseWriter.h"

class CSVWriter : public BaseWriter {
public:
    CSVWriter(const std::string& filename, int prec = 6);
    ~CSVWriter();
    void writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, double volume, bool intersects);
private:
    std::ofstream outFile;
    std::vector<std::string> headers;
    std::vector<std::string> entries;
    void setHeaders();
    void writeHeaders();
    void writeVertex(const Point& p);
};

#endif // CSVWRITER_H
