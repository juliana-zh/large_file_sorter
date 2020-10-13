#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <memory>
#include "sorter.h"

namespace SRT {


void Sorter::Run() { 
    ReadDivideSort();
    FillPriorityQueue();
    MergeWrite();
}

void Sorter::ReadDivideSort() {
    std::ifstream ifs(InFileName);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + InFileName);
    }

    std::string line;
    int curSizeBytes = 0;
    std::vector<std::string> buffer;
    buffer.reserve(25000);    
    while (!ifs.eof()) { 
        getline(ifs, line);        
        curSizeBytes += line.size();
        buffer.push_back(line); 
        if (curSizeBytes >= BufferLimitBytes) {
            std::sort(buffer.begin(), buffer.end());
            PutToFile(buffer);
            ++MaxCounterFileCreated;
            curSizeBytes = 0;
            buffer.clear();
        }					
	}
    if (curSizeBytes > 0) {
        PutToFile(buffer); 
        ++MaxCounterFileCreated;       
    }
}

void Sorter::PutToFile(const std::vector<std::string>& buffer) {
    std::filesystem::path workdir = std::filesystem::current_path();
    std::filesystem::create_directory(TMPDIR);
    std::filesystem::path filename = workdir / TMPDIR / std::to_string(MaxCounterFileCreated);     
    std::ofstream ofs(filename);  
    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open file: " + std::string(filename));
    }  
    for (int i = 0; i < (int)buffer.size(); ++i) {
        ofs << buffer[i] << std::endl;
    }
}

void Sorter::FillPriorityQueue() {    
    std::filesystem::path tmpdir = std::filesystem::current_path() / TMPDIR;
    for (int i = 0; i < MaxCounterFileCreated; ++i) {
        Elem el;
        std::filesystem::path filename = tmpdir / std::to_string(i); 
        el.ifs = std::make_shared<std::ifstream>(filename);
        if (!el.ifs->is_open()) {
            throw std::runtime_error("Failed to open file: " + std::string(filename));
        }
        getline(*(el.ifs), el.line);  
        LineToIfs.push(el);        
    }
}

void Sorter::MergeWrite() {
    std::ofstream ofs(OutFileName);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + OutFileName);
    }
    while (!LineToIfs.empty()) {
        Elem el = LineToIfs.top();
        ofs << el.line;
        if (LineToIfs.size() != 1) {
            ofs << "\n";
        }      
       

    }
}

}