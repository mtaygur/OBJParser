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


class LineSegmentOfVertexIndexPair
{
	public:
	LineSegmentOfVertexIndexPair(int first, int second)
	{
		if (first == second)
		{
			throw std::invalid_argument(
				"First and second vertex indices must be different");
		}
		this->data.insert(first);
		this->data.insert(second);
	}
	LineSegmentOfVertexIndexPair(std::pair<int, int> pair)
	{
		if (pair.first == pair.second)
		{
			throw std::invalid_argument(
				"First and second vertex indices must be different");
		}
		this->data.insert(pair.first);
		this->data.insert(pair.second);
	}

	bool operator==(const LineSegmentOfVertexIndexPair& other) const
	{
		return data == other.data;
	}


	std::pair<int, int> getPair() const
	{
		return std::make_pair(first(), second());
	}

	struct Hash
	{
		std::size_t operator()(const LineSegmentOfVertexIndexPair
							   & lineSegmentOfVertexIndexPair) const
		{
			boost::hash<std::pair<int, int>> hasher;
			return hasher(lineSegmentOfVertexIndexPair.getPair());

			return std::hash<int>()(lineSegmentOfVertexIndexPair.first()) ^
				std::hash<int>()(lineSegmentOfVertexIndexPair.second());
		}
	};


	private:
	int first() const { return *data.begin(); }
	int second() const { return *std::next(data.begin()); }
	std::set<int> data;
};

class LineSegmentOfVertexIndexPairSet
{
	/*
	public:
	void
		insert(const LineSegmentOfVertexIndexPair& lineSegmentOfVertexIndexPair)
	{


		data.insert(lineSegmentOfVertexIndexPair);
	}
	*/

	/*bool contains(
		LineSegmentOfVertexIndexPair lineSegmentOfVertexIndexPair) const
	{
		bool isStraightPairPresent = data.find(lineSegmentOfVertexIndexPair) != data.end();
		bool isReversedPairPresent = data.find(*lineSegmentOfVertexIndexPair.swapIndices()) != data.end();
		return isStraightPairPresent || isReversedPairPresent;
	}*/
	//std::size_t size() const { return data.size(); }
	//void clear() { data.clear(); }


	//private:
	//std::unordered_set<LineSegmentOfVertexIndexPair> data;


};
