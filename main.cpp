#include <fstream>
#include <iostream>

#include "sorter.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Failed. Wrong num of params. Example: ./sort <input_file> <limit_buf_bytes> <output_file>" << std::endl;
        return 1;
    }

    int limitBufBytes = 0;
    try {
        limitBufBytes = std::stoi(argv[2]);     
    } catch (const std::exception& ex) {
        std::cerr << "Failed: wrong limitBufBytes " << ex.what() << std::endl; 
        return 1;
    }

    if (limitBufBytes <= 0) {
        std::cerr << "Failed: limitBufBytes must be > 0" << std::endl;
        return 1;
    }

    try {
        SRT::Sorter s(argv[1], limitBufBytes, argv[3]);
        s.Run();
    } catch(std::exception& ex) {
        std::cout << "Failed to sort file: " << ex.what() << std::endl;
        return 1;
    }


	return 0;
}