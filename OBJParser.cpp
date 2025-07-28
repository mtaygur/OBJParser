#include "OBJParser.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/unordered_map.hpp>
#include <fstream>
#include <iostream>
#include <string>

std::vector<int> getVertexIdsFromLine(std::string &line) {
  // Remove comments from the line
  if (size_t commentCharPos = line.find_first_of('#');
      commentCharPos != std::string::npos) {
    line.resize(commentCharPos);
  }

  // Remove leading face prefix
  if (size_t fCharPos = line.find_first_of('f');
      fCharPos != std::string::npos) {
    line = line.substr(fCharPos + 1);
  } else {
    std::cout << "Invalid face definition: " << line << "\n";
    return {};
  }

  std::string lineFirstWord = line.substr(0, line.find_first_of(' '));
  size_t wordFirstSeparatorPos = lineFirstWord.find_first_of('/');
  size_t wordLastSeparatorPos = lineFirstWord.find_last_of('/');

  std::vector<int> vertexIds{};
  vertexIds.reserve(line.size()); // Reserve space (generously)
  static const boost::char_separator<char> sep(" ");
  boost::tokenizer<boost::char_separator<char>> tok(line, sep);

  if (wordFirstSeparatorPos == std::string::npos &&
      wordLastSeparatorPos == std::string::npos) {
    for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
      vertexIds.emplace_back(std::stoi(*beg));
    }
  } // first variant: line = "f  -324  213  213";
  else if (wordFirstSeparatorPos == wordLastSeparatorPos &&
           wordFirstSeparatorPos != std::string::npos &&
           wordLastSeparatorPos != std::string::npos) {
    for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
      vertexIds.emplace_back(
          std::stoi((*beg).substr(0, (*beg).find_first_of('/'))));
    }
  } // second variant: line = "f  435/324  765/213  987/213";
  else if (wordFirstSeparatorPos + 1 == wordLastSeparatorPos &&
           wordFirstSeparatorPos != std::string::npos &&
           wordLastSeparatorPos != std::string::npos) {
    for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
      vertexIds.emplace_back(
          std::stoi((*beg).substr(0, (*beg).find_first_of('/'))));
    }
  } // third variant: line = "f  435//324  765//213 ";
  else if (wordFirstSeparatorPos + 1 < wordLastSeparatorPos &&
           wordFirstSeparatorPos != std::string::npos &&
           wordLastSeparatorPos != std::string::npos) {
    for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
      vertexIds.emplace_back(
          std::stoi((*beg).substr(0, (*beg).find_first_of('/'))));
    }
  } // fourth variant: line = "f  435/789/324  765/25/213  987/9789/213
    // 5675/24234/2413";
  else {
    std::cout << "Invalid face format: " << line << "\n";
    return {};
  } // should not reach here
  return vertexIds;
}

OBJParser::OBJData::OBJData(const std::string &filename_)
    : filename(filename_) {
  if (filename.empty()) {
    std::cerr << "Filename cannot be empty\n" << std::flush;
    throw std::invalid_argument("Error: Filename cannot be empty");
  }

  // Check if the file exists
  if (boost::filesystem::path filepath{ filename }; 
      !boost::filesystem::exists(filepath)) {
    std::cerr << "File not found: " + filename << "\n" << std::flush;
    throw std::invalid_argument("Error: File not found: " + filename);
  }

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Unable to open file: " + filename << "\n" << std::flush;
    throw std::invalid_argument("Error: Unable to open file: " + filename);
  }

  // Count the number of newlines to get an estimate how many vertex/face
  // elements we would have
  long long newline_count{
      std::count(std::istreambuf_iterator<char>(file),
                 std::istreambuf_iterator<char>(), '\n') };

  // Return to the beginning of the file to start reading from the beginning
  file.clear();
  file.seekg(0);
  vertexList.reserve(newline_count / 8);
  vertexList.emplace_back();
  vertexNormalList.reserve(newline_count / 8);
  vertexNormalList.emplace_back();
  textureCoordinateList.reserve(newline_count / 8);
  textureCoordinateList.emplace_back();
  facesList.reserve(newline_count / 8);
  facesList.emplace_back();

  std::string line{};
  while (getline(file, line)) {

    // Remove spaces from the beginning and the end
    boost::algorithm::trim(line);

    std::vector<std::string> vertexString{};
    vertexString.reserve(4);
    boost::algorithm::split(vertexString, line, boost::is_any_of(" "),
                            boost::token_compress_on);
    std::string prefix = vertexString.at(0);

    if (prefix == "v") {
      vertexList.emplace_back(std::stod(vertexString.at(1)),
                                     std::stod(vertexString.at(2)),
                                     std::stod(vertexString.at(3)));
    } else if (prefix == "vn") {
      vertexNormalList.emplace_back(
          std::stod(vertexString.at(1)), std::stod(vertexString.at(2)),
          std::stod(vertexString.at(3)));
    } else if (prefix == "vt") {
      textureCoordinateList.emplace_back(
          std::stod(vertexString.at(1)),
          (vertexString.size() > 2) ? std::stod(vertexString.at(2)) : 0.,
          (vertexString.size() > 3) ? std::stod(vertexString.at(3)) : 0.);
    } else if (prefix == "f") {
      facesList.emplace_back(getVertexIdsFromLine(line));
    }
  }

  file.close();
}

OBJParser::LineSegment::LineSegment(int firstVertexIndex,
                                    int secondVertexIndex) {
  if (firstVertexIndex == secondVertexIndex) {
    throw std::invalid_argument(
        "First and second vertex indices must be different");
  }
  vertexIndexPair =
      std::make_pair(std::min(firstVertexIndex, secondVertexIndex),
                     std::max(firstVertexIndex, secondVertexIndex));
}

OBJParser::LineSegment::LineSegment(const std::pair<int, int> &pair) {
  if (pair.first == pair.second) {
    throw std::invalid_argument(
        "First and second vertex indices must be different");
  }
  vertexIndexPair = std::make_pair(std::min(pair.first, pair.second),
                                   std::max(pair.first, pair.second));
}

bool OBJParser::LineSegment::operator<(
    const OBJParser::LineSegment &other) const {
  return vertexIndexPair < other.vertexIndexPair;
}

std::vector<OBJParser::LineSegment> &
OBJParser::OBJData::calculateLineSegmentsList() {
    lineSegmentsList.clear();
    std::set<OBJParser::LineSegment> lineSegmentSet{ OBJParser::LineSegment{} };
  for (size_t faceIdx = 1; faceIdx < facesList.size(); ++faceIdx) {
    const auto &face = facesList.at(faceIdx);
    for (size_t i = 1; i < face.vertexIndices.size(); ++i) {
      auto lineSegment = OBJParser::LineSegment(std::make_pair(
          face.vertexIndices.at(i - 1), face.vertexIndices.at(i)));
      lineSegmentSet.insert(lineSegment);
    }
  }
  lineSegmentsList.assign(lineSegmentSet.begin(), lineSegmentSet.end());

  return lineSegmentsList;
}

std::unordered_map<size_t, std::vector<size_t>>& OBJParser::OBJData::calculateEdgeNeighborhoodList() {
  faceIdxToAdjacentFacesIdxListMap.clear();


  boost::unordered_map<std::set<int>, size_t>
      lineSegmentVertexIdxPairSetToLineIdxMap{};
  for (size_t i = 1; i < lineSegmentsList.size(); ++i) {
    const auto &lineSegment = lineSegmentsList.at(i);
    std::set<int> tempVertexIndexPairSet{lineSegment.vertexIndexPair.first,
                                         lineSegment.vertexIndexPair.second};
    assert(tempVertexIndexPairSet.size() == 2 &&
           "Line segment vertex index pair should contain exactly two "
           "elements.");
    lineSegmentVertexIdxPairSetToLineIdxMap[tempVertexIndexPairSet] = i;
  }

  std::unordered_map<size_t, std::vector<size_t>>
      lineIdxToAdjacentFacesIdxListMap{};
  size_t faceIdx = 0;
  for (const auto &face : facesList) {
    if (!face.vertexIndices.size())
      continue;

    auto tmp_faceVertexIndices = face.vertexIndices;
    tmp_faceVertexIndices.push_back(face.vertexIndices.front());
    for (size_t i = 1; i < tmp_faceVertexIndices.size(); ++i) {
      auto tmp_vertexIdxPairSet =
          std::set<int>{ tmp_faceVertexIndices.at(i - 1), tmp_faceVertexIndices.at(i)};
      auto iter =
          lineSegmentVertexIdxPairSetToLineIdxMap.find(tmp_vertexIdxPairSet);
      if (iter != lineSegmentVertexIdxPairSetToLineIdxMap.end()) {
        lineIdxToAdjacentFacesIdxListMap[iter->second].push_back(faceIdx);
      }
    }
    faceIdx++;
  }
  lineSegmentVertexIdxPairSetToLineIdxMap.clear();

  
  
  for (auto const &lineIdxToAdjacentFacesIdxList :
       lineIdxToAdjacentFacesIdxListMap) {
    const auto &adjacentFacesIdxList = lineIdxToAdjacentFacesIdxList.second;
    for (auto const &tmp_faceIdx : adjacentFacesIdxList) {
      for (auto const &adjacentFaceIdx : adjacentFacesIdxList) {
        if (tmp_faceIdx != adjacentFaceIdx) {
          faceIdxToAdjacentFacesIdxListMap[tmp_faceIdx].push_back(adjacentFaceIdx);
        }
      }
    }
  }

  return faceIdxToAdjacentFacesIdxListMap;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <filename> \n" << std::flush;
    return 1;
  }
  std::string filename = argv[1];

  OBJParser::OBJData data(filename);
  std::vector<OBJParser::LineSegment> lineSegmentsList =
      data.calculateLineSegmentsList();
  std::unordered_map<size_t, std::vector<size_t>>
      faceIdxToAdjacentFacesIdxListMap = data.calculateEdgeNeighborhoodList();

  return 0;
}
