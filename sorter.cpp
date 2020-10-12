#include <iostream>
#include <vector>
#include "sorter.h"

namespace SRT {


void Sorter::Run() { 
    ReadDivideSort();
    //MergeWrite();
}

void Sorter::ReadDivideSort() {
    std::ifstream ifs(LargeFileName);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + LargeFileName);
    }

    std::string line;
    int curSizeBytes = 0;
    std::vector<std::string> buffer;
    buffer.reverse(25000);
    int counter = 0;
    while (!ifs.eof()) { 
        getline(ifs, line);
        std::cout << "line: " << line << std::endl;
        curSizeBytes += line.size();
        buffer.push_back(line); 
        if (curSizeBytes >= BufferLimitBytes) {
            PutToFile(counter, buffer);
            ++counter;
            curSize = 0;
            buffer.clear();
        }					
	}
    if (curSizeBytes > 0) {
        PutToFile(counter, buffer);
    }

}



































}