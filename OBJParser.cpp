#include "OBJParser.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>



std::vector<int> getVertexIdsFromLine(std::string& line)
{
	// Remove comments from the line
	size_t commentCharPos = line.find_first_of("#");
	if (commentCharPos != std::string::npos)
		{
		line = line.substr(0, commentCharPos);
		}

	// Remove leading face prefix
	size_t fCharPos = line.find_first_of("f");
	if (fCharPos != std::string::npos)
	{
		line = line.substr(fCharPos + 1);
	}
	else
		{
		std::cerr << "Invalid face definition: " << line << std::endl;
		return {};
		}

	std::string lineFirstWord = line.substr(0, line.find_first_of(" "));
	size_t wordFirstSeparatorPos = lineFirstWord.find_first_of("/");
	size_t wordLastSeparatorPos = lineFirstWord.find_last_of("/");

	std::vector<int> vertexIds;
	vertexIds.reserve(line.size()); // Reserve space (generously)
	static const boost::char_separator<char> sep(" ");
	boost::tokenizer<boost::char_separator<char>> tok(line, sep);

	if (wordFirstSeparatorPos == std::string::npos && wordLastSeparatorPos == std::string::npos)
	{
		for (boost::tokenizer<boost::char_separator<char>>::iterator beg = tok.begin(); beg != tok.end(); ++beg)
		{
			vertexIds.emplace_back(std::stoi(*beg));
		}
	} // first variant: line = "f  -324  213  213";
	else if (wordFirstSeparatorPos == wordLastSeparatorPos && wordFirstSeparatorPos != std::string::npos && wordLastSeparatorPos != std::string::npos)
	{
		for (boost::tokenizer<boost::char_separator<char>>::iterator beg = tok.begin(); beg != tok.end(); ++beg)
		{
			vertexIds.emplace_back(std::stoi((*beg).substr(0, (*beg).find_first_of("/"))));
	}
	} // second variant: line = "f  435/324  765/213  987/213";
	else if (wordFirstSeparatorPos + 1 == wordLastSeparatorPos && wordFirstSeparatorPos != std::string::npos && wordLastSeparatorPos != std::string::npos)
	{
		for (boost::tokenizer<boost::char_separator<char>>::iterator beg = tok.begin(); beg != tok.end(); ++beg)
		{
			vertexIds.emplace_back(std::stoi((*beg).substr(0, (*beg).find_first_of("/"))));
		}
	} // third variant: line = "f  435//324  765//213 ";
	else if (wordFirstSeparatorPos + 1 < wordLastSeparatorPos && wordFirstSeparatorPos != std::string::npos && wordLastSeparatorPos != std::string::npos)
	{
		for (boost::tokenizer<boost::char_separator<char>>::iterator beg = tok.begin(); beg != tok.end(); ++beg)
		{
			vertexIds.emplace_back(std::stoi((*beg).substr(0, (*beg).find_first_of("/"))));
		}
	} // fourth variant: line = "f  435/789/324  765/25/213  987/9789/213  5675/24234/2413";
	else
	{
	std::cerr << "Invalid face format: " << line << std::endl;
	return {};
	} // should not reach here
	return vertexIds;

}

std::vector<LineSegment> calculateLineSegmentsList(const std::vector<Face>& facesList)
{
	std::set<LineSegment> lineSegmentSet{ LineSegment() };
	for (size_t faceIdx = 1; faceIdx < facesList.size(); ++faceIdx)
	{
		const auto& face = facesList.at(faceIdx);
		for (size_t i = 1; i < face.vertexIndices.size(); ++i)
		{
			auto lineSegment = LineSegment(std::make_pair(
				face.vertexIndices.at(i - 1), face.vertexIndices.at(i)));
			lineSegmentSet.insert(lineSegment);
		}
	}

	std::vector<LineSegment> lineSegmentsList(lineSegmentSet.begin(),
											  lineSegmentSet.end());

	return lineSegmentsList;
}

int main(int argc, char* argv[])
{

	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}
	std::string filename = argv[1];

	boost::filesystem::path filepath = filename;
	if (!boost::filesystem::exists(filepath))
	{
		std::cout << "File not found: " << filename << std::endl;
		return 1;
	}

	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Unable to open file: " << filename << std::endl;
		return 1;
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
	vertexList.reserve(newline_count / 8);
	vertexList.push_back(Vertex{});
	std::vector<VertexNormal> vertexNormalList{  };
	vertexNormalList.reserve(newline_count / 8);
	vertexNormalList.push_back(VertexNormal{});
	std::vector<TextureCoordinate> textureCoordinateList{  };
	textureCoordinateList.reserve(newline_count / 8);
	textureCoordinateList.push_back(TextureCoordinate{});

	std::vector<Face> facesList{};
	facesList.reserve(newline_count / 8);
	facesList.push_back(Face{});

	size_t lineCounter(0);
	std::string line;
	while (getline(file, line))
	{
		lineCounter++;

		// Remove spaces from the beginning and the end
		boost::algorithm::trim(line);

		std::vector<std::string> vertexString;
		vertexString.reserve(4);
		boost::algorithm::split(vertexString, line,
								boost::is_any_of(" "),
								boost::token_compress_on);
		auto prefix = vertexString.at(0);

		if (prefix == "v")
		{
			vertexList.emplace_back(Vertex(std::stof(vertexString.at(1)),
										   std::stof(vertexString.at(2)),
										   std::stof(vertexString.at(3))));
		}
		else if (prefix == "vn")
		{
			vertexNormalList.emplace_back(VertexNormal(std::stof(vertexString.at(1)),
													   std::stof(vertexString.at(2)),
													   std::stof(vertexString.at(3))));
		}
		else if (prefix == "vt")
		{
			textureCoordinateList.emplace_back(TextureCoordinate(std::stof(vertexString.at(1)),
																 (vertexString.size() > 2) ? std::stof(vertexString.at(2)) : 0.,
																 (vertexString.size() > 3) ? std::stof(vertexString.at(3)) : 0.));
		}

		else if (prefix == "f")
		{
			facesList.emplace_back(Face{ getVertexIdsFromLine(line) });
		}
	}
	auto lineSegmentsList = calculateLineSegmentsList(facesList);
	file.close();

	return 0;
}
