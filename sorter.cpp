#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <memory>
#include <chrono>
#include "sorter.h"

namespace SRT {


void Sorter::Run() {
    ReadDivideSort();
    FillPriorityQueue();
    MergeWrite();
    DeleteSorterTmpFiles();
}

void Sorter::ReadDivideSort() {
    std::ifstream ifs(InFileName);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + InFileName);
    }

    std::string line;
    std::vector<std::string> buffer;
    buffer.reserve(1000);
    int curSizeBytes = CountCurByteSize(buffer);

    while (!ifs.eof()) {
        getline(ifs, line);
        ++NumLines;
        buffer.push_back(line);
        curSizeBytes += sizeof(std::string);
        curSizeBytes += line.size();
        if (curSizeBytes >= BufferLimitBytes) {
            std::sort(buffer.begin(), buffer.end());
            PutToFile(buffer);
            ++MaxCounterFileCreated;
            buffer.clear();
            curSizeBytes = CountCurByteSize(buffer);
        }
    }
    if (!buffer.empty()) {
        std::sort(buffer.begin(), buffer.end());
        PutToFile(buffer);
        ++MaxCounterFileCreated;
    }
}

void Sorter::PutToFile(const std::vector<std::string>& buffer) {
    std::filesystem::path workdir = std::filesystem::current_path();
    std::filesystem::create_directory(TmpSorterDir);
    std::filesystem::path filename = workdir / TmpSorterDir / std::to_string(MaxCounterFileCreated);
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open file: " + std::string(filename));
    }
    if (buffer.empty()) {
        return;
    }
    for (int i = 0; i < (int)buffer.size() - 1; ++i) {
        ofs << buffer[i] << "\n";
    }
    ofs << buffer.back();
}

void Sorter::FillPriorityQueue() {
    std::filesystem::path dir = std::filesystem::current_path() / TmpSorterDir;
    for (int i = 0; i < MaxCounterFileCreated; ++i) {
        Elem el;
        std::filesystem::path filename = dir / std::to_string(i);
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
        --NumLines;
        ofs << el.line;
        if (NumLines != 0) {
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

int Sorter::CountCurByteSize(const std::vector<std::string>& v) {
    int size = 0;
    size += sizeof(std::vector<std::string>) + sizeof(std::string) * v.size();
    for(int i = 0; i < (int)v.size(); ++i) {
        size += v[i].length();
    }
    return size;
}

void Sorter::DeleteSorterTmpFiles() {
    std::filesystem::path pth = std::filesystem::current_path() / TmpSorterDir;
    std::filesystem::remove_all(pth);
}

std::string Sorter::GenerateName() {
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    return ".tmp_sorter_dir_" + std::to_string(ms.count());
}

}