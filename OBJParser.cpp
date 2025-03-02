#include "OBJParser.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>


std::vector<int> getVertexIdsFromLine(std::string& line)
{
	std::vector<int> vertexIds{};
	auto words_list_extract = [](std::string& line,
								 std::regex pattern) -> std::vector<std::string> {
		// Iterator to find all matches
		std::vector<std::string> words_list;
		auto words_begin = std::sregex_iterator(line.begin(), line.end(), pattern);
		auto words_end = std::sregex_iterator();
		for (std::sregex_iterator i = words_begin; i != words_end; ++i)
		{
			std::smatch match = *i;
			words_list.push_back(match.str());
		}
		return words_list;
	};

	//line = "f  435/789/324  765/25/213  987/9789/213  5675/24234/2413";
	std::regex pattern = std::regex(" -?\\d+/-?\\d+/-?\\d+");
	if (std::regex_search(line, pattern))
	{
		auto words_list = words_list_extract(line, pattern);
		for (auto& word : words_list)
		{
			size_t first_slash_idx = word.find_first_of("/");
			word = word.substr(0, first_slash_idx);
			int vertexId = std::stoi(word);
			vertexIds.push_back(vertexId);
		}
		return vertexIds;
	}

	//line = "f  435//324  765//213 ";
	pattern = std::regex(" -?\\d+//-?\\d+");
	if (std::regex_search(line, pattern))
	{
		auto words_list = words_list_extract(line, pattern);
		for (auto& word : words_list)
		{
			size_t first_slash_idx = word.find_first_of("/");
			word = word.substr(0, first_slash_idx);
			int vertexId = std::stoi(word);
			vertexIds.push_back(vertexId);
		}
		return vertexIds;
	}


	//line = "f  435/324  765/213  987/213";
	pattern = std::regex(" -?\\d+/-?\\d+");
	if (std::regex_search(line, pattern))
	{
		auto words_list = words_list_extract(line, pattern);
		for (auto& word : words_list)
		{
			size_t first_slash_idx = word.find_first_of("/");
			word = word.substr(0, first_slash_idx);
			int vertexId = std::stoi(word);
			vertexIds.push_back(vertexId);
		}
		return vertexIds;
	}

	//line = "f  -324  213  213";
	pattern = std::regex(" -?\\d+");
	if (std::regex_search(line, pattern))
	{
		auto words_list = words_list_extract(line, pattern);
		for (auto& word : words_list)
		{
			int vertexId = std::stoi(word);
			vertexIds.push_back(vertexId);
		}
		return vertexIds;
	}

	//should not reach here	
	std::cerr << "Invalid face format: " << line << std::endl;
	return {};
}


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

	// Return to the beginning of the file to start reading from the beginning
	file.clear();
	file.seekg(0);

	std::vector<Vertex> vertexList{ };
	vertexList.reserve(newline_count / 2);
	vertexList.push_back(Vertex{});
	std::vector<VertexNormal> vertexNormalList{  };
	vertexNormalList.reserve(newline_count / 2);
	vertexNormalList.push_back(VertexNormal{});
	std::vector<TextureCoordinate> textureCoordinateList{  };
	textureCoordinateList.reserve(newline_count / 2);
	textureCoordinateList.push_back(TextureCoordinate{});

	std::vector<Face> facesList{};
	facesList.reserve(newline_count / 2);
	facesList.push_back(Face{});


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
			facesList.push_back(Face{ getVertexIdsFromLine(line) });
		}
	}

	file.close();

	return 0;
}
