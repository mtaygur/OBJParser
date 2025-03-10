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
	LineSegment(int firstVertexIndex, int secondVertexIndex, const std::set<int>& linkedFaceIndices)
	{
		if (firstVertexIndex == secondVertexIndex)
		{
			throw std::invalid_argument(
				"First and second vertex indices must be different");
		}
		this->data.insert(firstVertexIndex);
		this->data.insert(secondVertexIndex);
		this->linkedFaceIndices.insert(linkedFaceIndices.begin(), linkedFaceIndices.end());
	}

	LineSegment(const std::pair<int, int>& pair, const std::set<int>& linkedFaceIndices)
	{
		if (pair.first == pair.second)
		{
			throw std::invalid_argument(
				"First and second vertex indices must be different");
		}
		this->data.insert(pair.first);
		this->data.insert(pair.second);
		this->linkedFaceIndices.insert(linkedFaceIndices.begin(), linkedFaceIndices.end());
	}

	LineSegment()
	{
	}

	bool operator==(const LineSegment& other) const
	{
		return (data == other.data);
	}


	std::pair<int, int> getPair() const
	{
		return std::make_pair(firstVertexIndex(), secondVertexIndex());
	}

	std::set<int> getLinkedFaceIndices() const
	{
		return linkedFaceIndices;
	}

	void addNewFaceIndex(int faceIndex)
	{
		linkedFaceIndices.insert(faceIndex);
	}

	struct Hash
	{
		std::size_t operator()(const LineSegment
							   & lineSegment) const
		{
			boost::hash<std::pair<int, int>> hasher;
			return hasher(lineSegment.getPair());
		}
	};


	private:
	int firstVertexIndex() const { return *data.begin(); }
	int secondVertexIndex() const { return *std::next(data.begin()); }
	std::set<int> data;
	std::set<int> linkedFaceIndices{};
};

