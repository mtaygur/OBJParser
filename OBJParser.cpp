#include "OBJParser.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>
#include <string>



std::vector<int> getVertexIdsFromLine(std::string& line)
{
	// Remove comments from the line
	if (size_t commentCharPos = line.find_first_of("#"); commentCharPos != std::string::npos)
	{
		line.resize(commentCharPos);
	}

	// Remove leading face prefix	
	if (size_t fCharPos = line.find_first_of("f"); fCharPos != std::string::npos)
	{
		line = line.substr(fCharPos + 1);
	}
	else
	{
		std::cout << "Invalid face definition: " << line << "\n";
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
		for (auto beg = tok.begin(); beg != tok.end(); ++beg)
		{
			vertexIds.emplace_back(std::stoi(*beg));
		}
	} // first variant: line = "f  -324  213  213";
	else if (wordFirstSeparatorPos == wordLastSeparatorPos && wordFirstSeparatorPos != std::string::npos && wordLastSeparatorPos != std::string::npos)
	{
		for (auto beg = tok.begin(); beg != tok.end(); ++beg)
		{
			vertexIds.emplace_back(std::stoi((*beg).substr(0, (*beg).find_first_of("/"))));
		}
	} // second variant: line = "f  435/324  765/213  987/213";
	else if (wordFirstSeparatorPos + 1 == wordLastSeparatorPos && wordFirstSeparatorPos != std::string::npos && wordLastSeparatorPos != std::string::npos)
	{
		for (auto beg = tok.begin(); beg != tok.end(); ++beg)
		{
			vertexIds.emplace_back(std::stoi((*beg).substr(0, (*beg).find_first_of("/"))));
		}
	} // third variant: line = "f  435//324  765//213 ";
	else if (wordFirstSeparatorPos + 1 < wordLastSeparatorPos && wordFirstSeparatorPos != std::string::npos && wordLastSeparatorPos != std::string::npos)
	{
		for (auto beg = tok.begin(); beg != tok.end(); ++beg)
		{
			vertexIds.emplace_back(std::stoi((*beg).substr(0, (*beg).find_first_of("/"))));
		}
	} // fourth variant: line = "f  435/789/324  765/25/213  987/9789/213  5675/24234/2413";
	else
	{
		std::cout << "Invalid face format: " << line << "\n";
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
		std::cout << "Usage: " << argv[0] << " <filename>\n";
		return 1;
	}
	std::string filename = argv[1];

	if (boost::filesystem::path filepath = filename; !boost::filesystem::exists(filepath))
	{
		std::cout << "File not found: " << filename << "\n";
		return 1;
	}

	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cout << "Unable to open file: " << filename << "\n";
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
	constexpr int lineCountDivider(8);
	vertexList.reserve(newline_count / lineCountDivider);
	vertexList.emplace_back();
	std::vector<VertexNormal> vertexNormalList{  };
	vertexNormalList.reserve(newline_count / lineCountDivider);
	vertexNormalList.emplace_back();
	std::vector<TextureCoordinate> textureCoordinateList{  };
	textureCoordinateList.reserve(newline_count / lineCountDivider);
	textureCoordinateList.emplace_back();

	std::vector<Face> facesList{};
	facesList.reserve(newline_count / lineCountDivider);
	facesList.emplace_back();

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
		std::string prefix = vertexString.at(0);

		if (prefix == "v")
		{
			vertexList.emplace_back(std::stof(vertexString.at(1)),
									std::stof(vertexString.at(2)),
									std::stof(vertexString.at(3)));
		}
		else if (prefix == "vn")
		{
			vertexNormalList.emplace_back(std::stof(vertexString.at(1)),
										  std::stof(vertexString.at(2)),
										  std::stof(vertexString.at(3)));
		}
		else if (prefix == "vt")
		{
			textureCoordinateList.emplace_back(std::stof(vertexString.at(1)),
											   (vertexString.size() > 2) ? std::stof(vertexString.at(2)) : 0.,
											   (vertexString.size() > 3) ? std::stof(vertexString.at(3)) : 0.);
		}

		else if (prefix == "f")
		{
			facesList.emplace_back(getVertexIdsFromLine(line));
		}
	}
	auto lineSegmentsList = calculateLineSegmentsList(facesList);
	file.close();

	return 0;
}
