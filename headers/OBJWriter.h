#pragma once


#include "BaseWriter.h"  
#include "Types.h"

class OBJWriter : public BaseWriter {

public:
    OBJWriter(const std::string& directory) : baseDir(directory) {}
    void writeEntry(const Tetrahedron& T1, const Tetrahedron& T2, bool intersectionStatus);
    void zipDirectory();
    
private:
    static int fileCounter;
    std::string baseDir;
    void writeTetrahedron(std::ofstream& outFile, const Tetrahedron& tetrahedron, int& vertexOffset);
    

};