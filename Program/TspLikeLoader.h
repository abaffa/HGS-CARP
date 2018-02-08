#ifndef TSPLIKELOADER_H
#define TSPLIKELOADER_H

#include <fstream>
#include <unordered_map>


#include "Vertex.h"
#include "Edge.h"

class TspLikeLoader
{
public:
	TspLikeLoader();

	bool load(std::string& fileName);

	std::unordered_map<int, Vertex> vertices;
	std::vector<Edge> edges;

	std::string name;

	int n_nodes;
	int n_arcs;
	int n_periods;

	int n_edgesRequired;
	int n_edgesNonRequired;

	int car_number;
	int car_capacity;

	//int depot_index = -1;

	std::vector<int> depot_indexes;


private:
	enum InstanceType
	{
		NONE,
		EUC_2D,
		GEO,
	};

	//std::vector<Vertex> vertices;

	InstanceType type = NONE;

	void loadEdges(std::ifstream& in);

	void loadNodes(std::ifstream& in);

	void loadDepots(std::ifstream& in);
		

};

#endif
