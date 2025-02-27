#include "OBJParser.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>


int main(int argc, char* argv[])
{

	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}
	std::string filename = argv[1];
	std::filesystem::path filepath = filename;
	if (!std::filesystem::exists(filepath))
	{
		std::cout << "File not found: " << filename << std::endl;
		return 1;
	}

	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Unable to open file: " << filename << std::endl;
		return false;
	}

	// Count the number of newlines to get an estimate how many vertex/face elements we would have
	std::size_t newline_count = std::count(
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>(),
		'\n'
	);

	std::vector<Vertex> vertexList{ };
	vertexList.reserve(newline_count / 2);
	vertexList.push_back(Vertex{});
	std::vector<VertexNormal> vertexNormalList{  };
	vertexNormalList.reserve(newline_count / 2);
	vertexNormalList.push_back(VertexNormal{});
	std::vector<TextureCoordinate> textureCoordinateList{  };
	textureCoordinateList.reserve(newline_count / 2);
	textureCoordinateList.push_back(TextureCoordinate{});

	std::string line;
	while (getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v")
		{
			Vertex vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			vertexList.push_back(vertex);
		}
		else if (prefix == "vn")
		{
			VertexNormal vertexNormal;
			iss >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;
			vertexNormalList.push_back(vertexNormal);
		}
		else if (prefix == "vt")
		{
			TextureCoordinate textureCoordinate;
			iss >> textureCoordinate.u >> textureCoordinate.v >> textureCoordinate.w;
			textureCoordinateList.push_back(textureCoordinate);
		}

		else if (prefix == "f")
		{


		}
	}

	file.close();

	return 0;
}
