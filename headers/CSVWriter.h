#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "Types.h"


class CSVWriter {
public:
    CSVWriter(const std::string& filename);
    ~CSVWriter();
    std::string static formatFilename(int number_of_entries);
    void writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, IntersectionType intersection_class, std::vector<Point> resulting_shape, bool intersects);
private:
    std::ofstream outFile;
    std::vector<std::string> headers;
    void initializeHeaders();
    void writeHeaders();
    void writeVertex(const Point& p);
};

#endif // CSVWRITER_H
