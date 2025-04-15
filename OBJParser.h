#pragma once
#include <boost/functional/hash.hpp>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <vector>


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


class LineSegment
{
	public:
	LineSegment(int firstVertexIndex, int secondVertexIndex)
	{
		if (firstVertexIndex == secondVertexIndex)
		{
			throw std::invalid_argument(
				"First and second vertex indices must be different");
		}
		vertexPair =
			std::make_pair(std::min(firstVertexIndex, secondVertexIndex), std::max(firstVertexIndex, secondVertexIndex));
	}

	explicit LineSegment(const std::pair<int, int>& pair)
	{
		if (pair.first == pair.second)
		{
			throw std::invalid_argument(
				"First and second vertex indices must be different");
		}
		vertexPair =
			std::make_pair(std::min(pair.first, pair.second), std::max(pair.first, pair.second));
	}

	LineSegment() = default;

	std::pair<int, int> getVertexPair() const { return vertexPair; }

	auto operator<=>(const LineSegment& other) const = default;

	private:
	std::pair<int, int> vertexPair{};
};

