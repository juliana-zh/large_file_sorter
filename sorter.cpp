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
    std::vector<std::string> buffer;
    int curSizeBytes = CountCurByteSize(buffer);
    std::cout << "curSizeBytes: " << curSizeBytes << std::endl;
    buffer.reserve(25000);    
    while (!ifs.eof()) { 
        getline(ifs, line);                      
        buffer.push_back(line); 
        curSizeBytes = CountCurByteSize(buffer);
        if (curSizeBytes >= BufferLimitBytes) {
            std::sort(buffer.begin(), buffer.end());
            PutToFile(buffer);
            ++MaxCounterFileCreated;            
            buffer.clear();            
        }					
	}
    if (!buffer.empty()) {
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
    if (buffer.empty()) {
        return;
    }
    for (int i = 0; i < (int)buffer.size() - 1; ++i) {
        ofs << buffer[i] << std::endl;
    }
    ofs << buffer.back();
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
    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open file: " + OutFileName);
    }
    while (!LineToIfs.empty()) {
        Elem el = LineToIfs.top();
        std::cout << "|" << el.line << "|" << std::endl;
        ofs << el.line;
        std::cout << "size: " << LineToIfs.size() << std::endl;
        if (LineToIfs.size() != 1) {
            ofs << "\n";
        }  

        if (el.ifs->eof()) {
            LineToIfs.pop();
            continue;
        }
  
        Elem newEl;
        newEl.ifs = el.ifs;
        getline(*(newEl.ifs), newEl.line);
        LineToIfs.pop();
        LineToIfs.push(newEl);
    }
}

void Sorter::CountCurByteSize(const std::vector<std::string>& v) {
    int size = 0;
    size += sizeof(std::vector<std::string>) + sizeof(std::string) * vector::size();
    for(int i = 0; i < (int)v.size(); ++i) {
        size += v[i].length();
    }
    return size;
}

}