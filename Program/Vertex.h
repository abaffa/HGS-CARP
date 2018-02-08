#ifndef VERTEX_H
#define VERTEX_H
#include <vector>

class Vertex
{
	friend class Loader;
	friend class Data;
public:
	Vertex() : index(-1), x(-1), y(-1) {}
	Vertex(int index, double x, double y) : index(index), x(x), y(y) {}

	int index;
	double x;
	double y;
	
	std::vector<int> edges;
};

#endif
