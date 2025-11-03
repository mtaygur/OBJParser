## Ways to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:
- Clear, descriptive title
- Steps to reproduce the problem
- Expected vs actual behavior
- Your environment (OS, compiler, versions)
- Sample OBJ file if relevant (or minimal reproduction case)

### Suggesting Enhancements

Feature requests are welcome! Please include:
- Clear description of the feature
- Use cases and motivation
- Any implementation ideas you have

### Code Contributions

1. **Fork the repository** and create your branch from `master`
2. **Set up your development environment** (see below)
3. **Make your changes** following our coding standards
4. **Add tests** for new functionality
5. **Ensure all tests pass**
6. **Update documentation** if needed
7. **Submit a pull request**

## Development Setup

### Prerequisites

- C++20 compatible compiler (GCC 10+, MSVC 2019+, or Clang 10+)
- CMake 3.15+
- vcpkg package manager
- Git

### Building from Source

```bash
# Clone your fork
git clone https://github.com/YOUR_USERNAME/OBJParser.git
cd OBJParser

# Configure with vcpkg
cmake --preset=linux-gcc-debug  # Or windows-msvc-debug, etc.

# Build
cmake --build --preset=linux-gcc-debug

# Run tests
ctest --preset=linux-gcc-debug
```

## Coding Standards

### Style Guidelines

- **Indentation**: Use tabs (existing codebase convention)
- **Naming**:
  - Classes/Structs: `PascalCase`
  - Functions/Methods: `camelCase`
  - Variables: `camelCase`
  - Constants: `UPPER_SNAKE_CASE`
- **Braces**: Opening brace on same line
- **Headers**: Use `#pragma once` for header guards
- **Includes**: Group and sort (standard library, third-party, project headers)

### Code Quality

- Write clear, self-documenting code
- Add comments for complex algorithms
- Keep functions focused and reasonably sized
- Avoid premature optimization
- Use modern C++20 features appropriately
- Handle errors properly (exceptions for construction, return values for operations)

### Example

```cpp
namespace OBJParser {
    struct MyNewFeature {
        double calculateSomething(const std::vector<Vertex>& vertices);
    };

    double MyNewFeature::calculateSomething(const std::vector<Vertex>& vertices) {
        if (vertices.empty()) {
            throw std::invalid_argument("Vertex list cannot be empty");
        }

        double result = 0.0;
        // ... implementation
        return result;
    }
}
```

## Testing

### Writing Tests

- Add unit tests for all new features
- Use Google Test framework
- Place tests in `test/` directory
- Test both success cases and error conditions
- Use meaningful test names: `TestName_Scenario_ExpectedBehavior`

### Example Test

```cpp
TEST(OBJParserTest, ParseCube_ValidFile_ReturnsCorrectVertexCount) {
    OBJParser::OBJData data("examples/models/cube.obj");
    EXPECT_EQ(data.vertexList.size(), 9);  // 8 vertices + 1 dummy
}

TEST(OBJParserTest, Constructor_InvalidFile_ThrowsException) {
    EXPECT_THROW(OBJParser::OBJData("nonexistent.obj"), std::invalid_argument);
}
```

### Running Tests

```bash
# Build tests
cmake --build build --target OBJParser_tests

# Run all tests
./build/OBJParser_tests

# Run specific test
./build/OBJParser_tests --gtest_filter=OBJParserTest.ParseCube*
```

## Documentation

- Update README.md for user-facing changes
- Add inline comments for complex code
- Update API documentation for new public interfaces
- Include code examples for new features

## Pull Request Process

1. **Update CHANGELOG.md**
2. **Ensure CI passes**
3. **Request review** 
4. **Address feedback** 

### PR Description Template

```markdown
## Description
Brief description of what this PR does

## Motivation
Why is this change needed?

## Changes
- List of changes made
- Each change on its own line

## Testing
How was this tested?

## Checklist
- [ ] Tests added/updated
- [ ] Documentation updated
- [ ] CHANGELOG.md updated
- [ ] All tests pass locally
- [ ] Code follows project style
```

## License

By contributing to OBJParser, you agree that your contributions will be licensed under the GNU General Public License v3.0.
