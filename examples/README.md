# OBJParser Examples

This directory contains example OBJ models and usage examples for the OBJParser library.

## Sample Models

The `models/` directory contains simple OBJ files you can use for testing and learning:

| Model | Vertices | Faces | Description |
|-------|----------|-------|-------------|
| `triangle.obj` | 3 | 1 | Simplest possible mesh - single triangle |
| `tetrahedron.obj` | 4 | 4 | Simple 3D shape - triangular pyramid |
| `pyramid.obj` | 5 | 5 | Square pyramid with quad base and triangle sides |
| `cube.obj` | 8 | 6 | Unit cube centered at origin |

## Basic Usage Example

```cpp
#include "OBJParser.h"
#include <iostream>

int main() {
    // Load a sample model
    OBJParser::OBJData model("examples/models/cube.obj");

    // Print basic information
    std::cout << "Model: " << model.filename << std::endl;
    std::cout << "Vertices: " << model.vertexList.size() - 1 << std::endl;  // -1 for dummy vertex
    std::cout << "Faces: " << model.facesList.size() - 1 << std::endl;      // -1 for dummy face

    // Access vertex data
    std::cout << "\nFirst 3 vertices:" << std::endl;
    for (size_t i = 1; i <= 3 && i < model.vertexList.size(); ++i) {
        const auto& v = model.vertexList[i];
        std::cout << "  v" << i << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
    }

    // Calculate edge segments
    auto& edges = model.calculateLineSegmentsList();
    std::cout << "\nUnique edges: " << edges.size() - 1 << std::endl;  // -1 for dummy

    // Calculate face adjacency
    auto& adjacency = model.calculateEdgeNeighborhoodList();
    std::cout << "Faces with neighbors: " << adjacency.size() << std::endl;

    return 0;
}
```

## Advanced Examples

### Example 1: Finding Bounding Box

```cpp
#include "OBJParser.h"
#include <limits>
#include <iostream>

void printBoundingBox(const OBJParser::OBJData& model) {
    if (model.vertexList.size() <= 1) {
        std::cout << "No vertices in model" << std::endl;
        return;
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double minZ = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();
    double maxZ = std::numeric_limits<double>::lowest();

    // Skip first element (dummy vertex at index 0)
    for (size_t i = 1; i < model.vertexList.size(); ++i) {
        const auto& v = model.vertexList[i];
        minX = std::min(minX, v.x);
        minY = std::min(minY, v.y);
        minZ = std::min(minZ, v.z);
        maxX = std::max(maxX, v.x);
        maxY = std::max(maxY, v.y);
        maxZ = std::max(maxZ, v.z);
    }

    std::cout << "Bounding Box:" << std::endl;
    std::cout << "  Min: (" << minX << ", " << minY << ", " << minZ << ")" << std::endl;
    std::cout << "  Max: (" << maxX << ", " << maxY << ", " << maxZ << ")" << std::endl;
    std::cout << "  Size: (" << (maxX-minX) << " x " << (maxY-minY) << " x " << (maxZ-minZ) << ")" << std::endl;
}

int main() {
    OBJParser::OBJData cube("examples/models/cube.obj");
    printBoundingBox(cube);
    return 0;
}
```

### Example 2: Analyzing Face Adjacency

```cpp
#include "OBJParser.h"
#include <iostream>

void analyzeAdjacency(OBJParser::OBJData& model) {
    auto& adjacency = model.calculateEdgeNeighborhoodList();

    std::cout << "Face Adjacency Analysis:" << std::endl;
    for (const auto& [faceIdx, neighbors] : adjacency) {
        std::cout << "  Face " << faceIdx << " has " << neighbors.size()
                  << " neighbor(s): ";
        for (size_t neighborIdx : neighbors) {
            std::cout << neighborIdx << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    OBJParser::OBJData pyramid("examples/models/pyramid.obj");
    analyzeAdjacency(pyramid);
    return 0;
}
```

### Example 3: Detecting Boundary Edges

```cpp
#include "OBJParser.h"
#include <iostream>
#include <unordered_map>
#include <set>

void findBoundaryEdges(OBJParser::OBJData& model) {
    // Map each edge to the faces that use it
    std::map<std::pair<int,int>, std::vector<size_t>> edgeToFaces;

    for (size_t faceIdx = 1; faceIdx < model.facesList.size(); ++faceIdx) {
        const auto& face = model.facesList[faceIdx];

        for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
            size_t next = (i + 1) % face.vertexIndices.size();
            int v1 = face.vertexIndices[i];
            int v2 = face.vertexIndices[next];

            // Create ordered edge
            auto edge = std::make_pair(std::min(v1, v2), std::max(v1, v2));
            edgeToFaces[edge].push_back(faceIdx);
        }
    }

    // Boundary edges are used by only one face
    std::cout << "Boundary Edges:" << std::endl;
    int boundaryCount = 0;
    for (const auto& [edge, faces] : edgeToFaces) {
        if (faces.size() == 1) {
            std::cout << "  Edge (" << edge.first << "-" << edge.second
                      << ") on face " << faces[0] << std::endl;
            boundaryCount++;
        }
    }
    std::cout << "Total boundary edges: " << boundaryCount << std::endl;
}

int main() {
    OBJParser::OBJData model("examples/models/pyramid.obj");
    findBoundaryEdges(model);
    return 0;
}
```

## Building the Examples

To build your own examples that use the library:

```bash
# Create your example file
cat > my_example.cpp << 'EOF'
#include "OBJParser.h"
#include <iostream>

int main() {
    OBJParser::OBJData model("examples/models/cube.obj");
    std::cout << "Loaded " << model.vertexList.size() - 1 << " vertices" << std::endl;
    return 0;
}
EOF

# Compile and link against OBJParser_lib
g++ -std=c++20 my_example.cpp -o my_example \
    -I./src \
    -L./build \
    -lOBJParser_lib \
    -lboost_filesystem

# Run
./my_example
```

Or use CMake:

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyExample)

set(CMAKE_CXX_STANDARD 20)

# Find OBJParser (if installed) or use add_subdirectory
add_subdirectory(path/to/OBJParser)

add_executable(my_example my_example.cpp)
target_link_libraries(my_example PRIVATE OBJParser_lib)
```

## Tips for Using Sample Models

- **triangle.obj**: Perfect for testing basic parsing functionality
- **tetrahedron.obj**: Good for testing edge adjacency (closed mesh, all edges shared)
- **pyramid.obj**: Tests mixed face types (quads + triangles) and boundary edges
- **cube.obj**: Classic test case for face adjacency on a closed manifold mesh

## Contributing Examples

Have an interesting use case? We'd love to see it! Please submit a pull request with:
- Your example code with clear comments
- Sample output showing what it does
- Brief description in this README
