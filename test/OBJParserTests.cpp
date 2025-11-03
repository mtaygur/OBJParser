#include <gtest/gtest.h>
#include "OBJParser.h"
#include <fstream>
#include <filesystem>

TEST(OBJParser_Parse, SimpleCube) {
    namespace fs = std::filesystem;
    fs::path tmp = fs::temp_directory_path() / "simple_cube_for_test.obj";

    const char* cube =
        "# Simple cube\n"
        "v 1.000000 -1.000000 -1.000000\n"
        "v 1.000000 -1.000000 1.000000\n"
        "v -1.000000 -1.000000 1.000000\n"
        "v -1.000000 -1.000000 -1.000000\n"
        "v 1.000000 1.000000 -0.999999\n"
        "v 0.999999 1.000000 1.000001\n"
        "v -1.000000 1.000000 1.000000\n"
        "v -1.000000 1.000000 -1.000000\n"
        "f 1 2 3 4\n"
        "f 5 8 7 6\n"
        "f 1 5 6 2\n"
        "f 2 6 7 3\n"
        "f 3 7 8 4\n"
        "f 5 1 4 8\n";

    {
        std::ofstream ofs(tmp);
        ASSERT_TRUE(ofs.is_open());
        ofs << cube;
    }

    // Use the library API directly
    OBJParser::OBJData data(tmp.string());

    // parser reserves a dummy at index 0
    EXPECT_EQ(data.vertexList.size(), static_cast<size_t>(8 + 1));
    EXPECT_EQ(data.facesList.size(), static_cast<size_t>(6 + 1));

    auto &lines = data.calculateLineSegmentsList();
    EXPECT_FALSE(lines.empty());

    auto &adj = data.calculateEdgeNeighborhoodList();
    EXPECT_GE(adj.size(), static_cast<size_t>(0));

    std::error_code ec;
    fs::remove(tmp, ec);
}