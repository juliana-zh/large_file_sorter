#include <fstream>
#include <iostream>

#include "sorter.h"

int main(int argc, char **argv) {

    try {
        SRT::Sorter s;
        s.Run();
    } catch(std::exception& ex) {
        std::cout << "Failed to sort file: " << ex.what() << std::endl;
    }


	return 0;
}