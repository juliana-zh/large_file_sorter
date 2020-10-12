#pragma once

#include <string>
#include <fstream>

namespace SRT {

struct Elem {
	std::string line;
	std::ifstream* ifs;
};

class Sorter {
public:
    Sorter (const std::string& largeFileName, int bufferLimitBytes) 
        : LargeFileName(largeFileName)
        , BufferLimitBytes(bufferLimitBytes)

	void Run();

private:
    void ReadDivideSort();
    //void MergeWrite();
private:
    std::string LargeFileName = "in";
    int BufferLimitBytes = 10 * 1024 * 1024;
};



}