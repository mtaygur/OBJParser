#pragma once
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace OBJParser
{
	struct Vertex
	{
		double x{ 0.0 };
		double y{ 0.0 };
		double z{ 0.0 };
	};

	struct VertexNormal
	{
		double x{ 0.0 };
		double y{ 0.0 };
		double z{ 0.0 };
	};

	struct TextureCoordinate
	{
		double u{ 0.0 };
		double v{ 0.0 };
		double w{ 0.0 };
	};

	struct Face
	{
		std::vector<int> vertexIndices{};
	};

	struct LineSegment
	{
		LineSegment(int firstVertexIndex, int secondVertexIndex);
		LineSegment(const std::pair<int, int>& pair);
		LineSegment() {}

		bool operator<(const LineSegment& other) const;

		std::pair<int, int> vertexIndexPair{};
	};

	struct OBJData
	{
		OBJData(const std::string& filename_);
		OBJData(const OBJData& other) = default;
		OBJData(OBJData&& other) = default;

		std::vector<LineSegment>& calculateLineSegmentsList();

		std::unordered_map<size_t, std::vector<size_t>>& calculateEdgeNeighborhoodList();

		std::string filename{};
		std::vector<Vertex> vertexList{};
		std::vector<VertexNormal> vertexNormalList{};
		std::vector<TextureCoordinate> textureCoordinateList{};
		std::vector<Face> facesList{};
		std::vector<LineSegment> lineSegmentsList{};
		std::unordered_map<size_t, std::vector<size_t>>
			faceIdxToAdjacentFacesIdxListMap{};
	};

}