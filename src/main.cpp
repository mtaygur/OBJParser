#include "OBJParser.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n" << std::flush;
        return 1;
    }
    std::string filename = argv[1];

    OBJParser::OBJData data(filename);
    auto lines = data.calculateLineSegmentsList();
    auto adj = data.calculateEdgeNeighborhoodList();

    return 0;
}