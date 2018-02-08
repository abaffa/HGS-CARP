#ifndef EDGE_H
#define EDGE_H

class Edge
{
public:
	Edge() : index(-1), from(-1), to(-1), cost(0), baseline(0), demand(0) {}
	Edge(int index, int from, int to, int cost, int demand) : index(index), from(from), to(to), cost(cost), baseline(baseline), demand(demand) {}

	int getOther(int right) const { return (from == right) ? to : from; }

	bool operator<(const Edge& right) const { return (cost < right.cost); }

	int index;
	int from;
	int to;
	double cost;
	int demand;
	int baseline;
	std::vector<double> speed_model;
};

#endif
