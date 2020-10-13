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
        , TmpSorterDir(GenerateName())
        {}

    void Run();

private:
    void ReadDivideSort();
    void FillPriorityQueue();
    void PutToFile(const std::vector<std::string>& buffer);
    void MergeWrite();
    int CountCurByteSize(const std::vector<std::string>& v);
    void DeleteSorterTmpFiles();
    std::string GenerateName();
private:
    std::string InFileName;
    int BufferLimitBytes = 10 * 1024 * 1024;
    std::string OutFileName;
    int MaxCounterFileCreated = 0;
    std::string TmpSorterDir;
    std::priority_queue<Elem> LineToIfs;
    int NumLines = 0;
};

}