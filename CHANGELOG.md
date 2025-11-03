# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Comprehensive README with full documentation, API reference, and usage examples
- Example models directory with sample OBJ files (cube, pyramid, tetrahedron, triangle)
- Examples README with usage examples and code snippets
- Contributing guidelines (CONTRIBUTING.md)
- Code of Conduct (CODE_OF_CONDUCT.md)
- Issue and pull request templates
- This CHANGELOG file

## [0.1.0] - 2024-01-XX (Initial Features)

### Added
- Core OBJ file parser supporting vertices, normals, and texture coordinates
- Support for multiple face format variants (v, v/vt, v//vn, v/vt/vn)
- Line segment extraction from face geometry
- Face adjacency calculation (edge neighborhood analysis)
- Unit tests using Google Test
- CMake build system with vcpkg integration
- Cross-platform support (Linux, Windows with MSVC and MinGW)
- CI/CD pipelines for automated testing
- SonarCloud and Codacy integration for code quality

### Features
- `OBJData` class for parsing and storing OBJ file data
- `calculateLineSegmentsList()` method for edge extraction
- `calculateEdgeNeighborhoodList()` method for face adjacency
- Robust error handling with descriptive exceptions
- Performance optimization with memory pre-allocation

[Unreleased]: https://github.com/mtaygur/OBJParser/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/mtaygur/OBJParser/releases/tag/v0.1.0
