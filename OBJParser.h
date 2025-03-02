#pragma once
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
