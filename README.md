# OBJParser

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)

[![Linux](https://github.com/mtaygur/OBJParser/workflows/linux-cmake-vcpkg-build/badge.svg)](https://github.com/mtaygur/OBJParser/actions?query=workflow%3Alinux)
[![Windows](https://github.com/mtaygur/OBJParser/workflows/windows-cmake-vcpkg-build/badge.svg)](https://github.com/mtaygur/OBJParser/actions?query=workflow%3Awindows)
![GitHub last commit (branch)](https://img.shields.io/github/last-commit/mtaygur/OBJParser/master)

[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=mtaygur_OBJParser&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=mtaygur_OBJParser)
[![Sonar Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=mtaygur_OBJParser&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=mtaygur_OBJParser)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/eba9aa15be974a12b987f19c3bf1de0c)](https://app.codacy.com/gh/mtaygur/OBJParser/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

A modern C++20 library for parsing Wavefront OBJ 3D model files and performing geometric analysis. OBJParser extracts vertices, faces, normals, and texture coordinates while providing utilities to compute edge segments and face adjacency relationships.

## Features

- **Complete OBJ Parsing**: Supports vertices, vertex normals, texture coordinates, and faces
- **Multiple Face Format Support**: Handles all common OBJ face definition variants:
  - Vertex indices only: `f v1 v2 v3`
  - With texture coordinates: `f v1/vt1 v2/vt2 v3/vt3`
  - With normals: `f v1//vn1 v2//vn2 v3//vn3`
  - Combined: `f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3`
- **Edge Extraction**: Automatically computes unique line segments from face geometry
- **Face Adjacency Analysis**: Determines which faces share edges for neighborhood queries
- **Performance Optimized**: Pre-allocates memory based on file size for efficient parsing
- **Robust Error Handling**: Validates input and provides informative error messages
- **Cross-Platform**: Builds on Windows (MSVC, MinGW) and Linux (GCC)

## Requirements

- **C++20** compatible compiler (GCC 10+, MSVC 2019+, or MinGW)
- **CMake** 3.15 or higher
- **vcpkg** for dependency management
- **Boost** 1.63+ (filesystem, algorithm, tokenizer)
- **Google Test** (optional, for unit tests)

## Building

### Using vcpkg and CMake Presets (Recommended)

```bash
# Clone the repository
git clone https://github.com/mtaygur/OBJParser.git
cd OBJParser

# Configure with vcpkg (Linux example)
cmake --preset=linux-gcc-debug

# Build
cmake --build --preset=linux-gcc-debug

# Run tests (optional)
ctest --preset=linux-gcc-debug
```

### Available Presets

- **Linux**: `linux-gcc-debug`, `linux-gcc-release`
- **Windows (MSVC)**: `windows-msvc-debug`, `windows-msvc-release`
- **Windows (MinGW)**: `windows-mingw-debug`, `windows-mingw-release`

### Manual Build

```bash
# Configure
cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run tests
cd build && ctest
```

## Usage

### As a Library

```cpp
#include "OBJParser.h"

int main() {
    // Parse an OBJ file
    OBJParser::OBJData data("model.obj");

    // Access parsed data
    std::cout << "Vertices: " << data.vertexList.size() << std::endl;
    std::cout << "Faces: " << data.facesList.size() << std::endl;

    // Calculate edge segments
    auto& edges = data.calculateLineSegmentsList();
    std::cout << "Unique edges: " << edges.size() << std::endl;

    // Calculate face adjacency
    auto& adjacency = data.calculateEdgeNeighborhoodList();

    // Access a specific vertex
    const auto& v = data.vertexList[0];
    std::cout << "First vertex: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;

    return 0;
}
```

### Command-Line Tool

```bash
# Run the example executable
./OBJParser_exe path/to/model.obj
```

## API Reference

### Core Data Structures

#### `OBJParser::Vertex`
Represents a 3D point in space.
```cpp
struct Vertex {
    double x, y, z;
};
```

#### `OBJParser::VertexNormal`
Represents a vertex normal vector.
```cpp
struct VertexNormal {
    double x, y, z;
};
```

#### `OBJParser::TextureCoordinate`
Represents UV (and optional W) texture coordinates.
```cpp
struct TextureCoordinate {
    double u, v, w;
};
```

#### `OBJParser::Face`
Represents a polygon face with vertex indices.
```cpp
struct Face {
    std::vector<int> vertexIndices;
};
```

#### `OBJParser::LineSegment`
Represents an edge between two vertices with ordered indices.
```cpp
struct LineSegment {
    LineSegment(int firstVertexIndex, int secondVertexIndex);
    std::pair<int, int> vertexIndexPair;
};
```

### Main Class

#### `OBJParser::OBJData`
Main parser class that loads and analyzes OBJ files.

**Constructor:**
```cpp
OBJData(const std::string& filename);  // Throws std::invalid_argument if file doesn't exist
```

**Methods:**
```cpp
// Computes all unique edge segments from faces
std::vector<LineSegment>& calculateLineSegmentsList();

// Computes face adjacency (maps face index -> adjacent face indices)
std::unordered_map<size_t, std::vector<size_t>>& calculateEdgeNeighborhoodList();
```

**Public Members:**
```cpp
std::string filename;
std::vector<Vertex> vertexList;
std::vector<VertexNormal> vertexNormalList;
std::vector<TextureCoordinate> textureCoordinateList;
std::vector<Face> facesList;
std::vector<LineSegment> lineSegmentsList;
std::unordered_map<size_t, std::vector<size_t>> faceIdxToAdjacentFacesIdxListMap;
```

## Testing

The project includes unit tests using Google Test:

```bash
# Build and run tests
cmake --build build --target OBJParser_tests
./build/OBJParser_tests
```

## Project Structure

```
OBJParser/
├── src/
│   ├── OBJParser.h          # Public API header
│   ├── OBJParser.cpp        # Implementation
│   └── main.cpp             # Example application
├── test/
│   └── OBJParserTests.cpp   # Unit tests
├── CMakeLists.txt           # Build configuration
├── CMakePresets.json        # Build presets
├── vcpkg.json               # Dependency manifest
└── README.md                # This file
```

## Continuous Integration

Automated builds and tests run on:
- **Linux**: Ubuntu with GCC
- **Windows**: MSVC and MinGW

Code quality is monitored via:
- **SonarCloud**: Code quality and security analysis
- **Codacy**: Automated code review

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE.txt](LICENSE.txt) file for details.

## Contributing

Contributions are welcome! Please ensure that:
1. Code follows the existing style
2. All tests pass
3. New features include unit tests
4. Commit messages are clear and descriptive

## Author

Maintained by [mtaygur](https://github.com/mtaygur)
