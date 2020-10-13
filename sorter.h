#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <queue>

namespace SRT {

struct Elem {
	std::string line;
	std::shared_ptr<std::ifstream> ifs;
    bool operator< (const Elem& b) const {
        return line > b.line;
    }
};

class Sorter {
public:
    Sorter (const std::string& inFileName, int bufferLimitBytes, const std::string& outFileName) 
        : InFileName(inFileName)
        , BufferLimitBytes(bufferLimitBytes)
        , OutFileName(outFileName)
        {}

	void Run();

private:
    void ReadDivideSort();    
    void FillPriorityQueue();
    void PutToFile(const std::vector<std::string>& buffer);
    void MergeWrite();
    void CountCurByteSize(const std::vector<std::string>& v);
private:
    std::string InFileName;
    int BufferLimitBytes = 10 * 1024 * 1024;
    std::string OutFileName;
    int MaxCounterFileCreated = 0;
    const std::string TMPDIR = "tmpdir_sorter";    
    std::priority_queue<Elem> LineToIfs;
};



}