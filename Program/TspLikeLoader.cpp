#define _USE_MATH_DEFINES
#include "TspLikeLoader.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

TspLikeLoader::TspLikeLoader() {}

bool TspLikeLoader::load(string& fileName)
{

	ifstream in(fileName.c_str(), ifstream::in);
	if (!in.good()) return false;

	string left;
	string right;
	char colon;
	int value;

	bool edgesLoaded = false;

	while (true)
	{
		in >> left;
		if (left == "EOF") break;

		if (left == "DIMENSION")
		{
			in >> colon;

			in >> value;
			n_nodes = value;

			in >> value;
			n_arcs = value;

			//vertices.resize(n_nodes);
			edges.resize(n_arcs);

		}
		else if (left == "CAPACITY")
		{
			in >> colon >> value;
			car_capacity = value;
		}
		else if (left == "PERIODS")
		{
			in >> colon >> value;
			n_periods = value;
		}
		else if (left == "VEHICLES")
		{
			in >> colon >> value;
			car_number = value;
		}
		else if (left == "EDGE_WEIGHT_TYPE")
		{
			in >> colon >> right;
			if (right == "EUC_2D") type = EUC_2D;
			else if (right == "GEO") type = GEO;
			else
				throw string(" Invalid EDGE_WEIGHT_TYPE ");
		}
		else if (left == "NODE_COORD_SECTION")
			loadNodes(in);
		else if (left == "EDGE_DATA_SECTION") {
			if (!edgesLoaded) {
				loadEdges(in);
				edgesLoaded = true;
			}
			else
				throw string(" Invalid EDGE_DATA_SECTION ");
		}
		else if (left == "DEPOT_SECTION")
			loadDepots(in);
	}

	if (car_number == 0)
	{
		int q_total = 0; //demand
		for (int e = 0; e < (int)edges.size(); e++)
			q_total += edges[e].demand;

		// fix the fact that the instances dont have vehicle info yet
		car_number = (int)(ceil(q_total / (double)car_capacity) + 1e-7);//m_LB = \ceil(q_total / Q

		cout << "m_LB: " << car_number << endl;
		cout << "Q: " << car_capacity << endl;
		cout << "q_total: " << q_total << endl;
	}

	if(depot_indexes.size() == 0)
		depot_indexes.push_back(0);


	return true;
}



void TspLikeLoader::loadDepots(std::ifstream& in) {


	int index;

	while (true)
	{
		in >> index;
		if (index == -1)
			break;
		
		if(vertices.find(index) != vertices.end())
			depot_indexes.push_back(index);
		else
			throw string(" Invalid DEPOT_SECTION ");

	}
}

void TspLikeLoader::loadNodes(std::ifstream& in) {
	//vector<pair<double, double>> points(vertices.size());
	unordered_map<int, pair<double, double>> points;
	for (int v = 0; v < n_nodes; v++)
	{
		int vertex;
		double x, y;
		in >> vertex >> x >> y;
		vertices[vertex].index = vertex;
		vertices[vertex].x = x;
		vertices[vertex].y = y;
	}
}

double EuclideanDistance(pair<double, double> p1, pair<double, double> p2)
{
	double a = p1.first - p2.first;
	double b = p1.second - p2.second;

	return sqrt(a * a + b * b);
}

double ToRadians(double val)
{
	return (M_PI / 180) * val;
}


double GeoDistance(pair<double, double> p1, pair<double, double> p2)
{
	double lat_1 = p1.first;
	double lat_2 = p2.first;
	double lon_1 = p1.second;
	double lon_2 = p2.second;

	double dLon = lon_2 - lon_1;
	double dLat = lat_2 - lat_1;

	lat_1 = ToRadians(lat_1);
	lon_1 = ToRadians(lon_1);
	lat_2 = ToRadians(lat_2);
	lon_2 = ToRadians(lon_2);
	dLon = ToRadians(dLon);
	dLat = ToRadians(dLat);

	double r = (6378137.0 / 1000.0);

	double a = (sin(dLat / 2) * sin(dLat / 2) +
		cos(lat_1) * cos(lat_2) *
		sin(dLon / 2) * sin(dLon / 2));

	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return c * r;
}


void TspLikeLoader::loadEdges(ifstream& in)
{
	n_edgesRequired = 0;
	n_edgesNonRequired = 0;

	for (int e = 0; e < n_arcs; e++)
	{
		int vertex1;
		int vertex2;
		int demand;
		int baseline;
		double cost;

		in >> vertex1 >> vertex2 >> demand >> baseline;

		if (type == EUC_2D)
			cost = EuclideanDistance(
				make_pair(vertices[vertex1].x, vertices[vertex1].y),
				make_pair(vertices[vertex2].x, vertices[vertex2].y));

			/*
			cost = (int)(EuclideanDistance(
				make_pair(vertices[vertex1].x, vertices[vertex1].y),
				make_pair(vertices[vertex2].x, vertices[vertex2].y)) + 0.5);
				*/
		else if (type == GEO)
			cost = GeoDistance(
				make_pair(vertices[vertex1].x, vertices[vertex1].y),
				make_pair(vertices[vertex2].x, vertices[vertex2].y));

			/*
			cost = (int)(GeoDistance(
				make_pair(vertices[vertex1].x, vertices[vertex1].y),
				make_pair(vertices[vertex2].x, vertices[vertex2].y)) * 1000 + 0.5);
				*/
		else
			throw string(" Invalid EDGE_WEIGHT_TYPE ");

		vertices[vertex1].edges.push_back(e);

		edges[e].from = vertex1;
		edges[e].to = vertex2;
		edges[e].cost = cost;
		edges[e].demand = demand;
		edges[e].index = e;

		if (n_periods > 0) {
			vector<double> speed_model(n_periods);
			for (int p = 0; p < n_periods; p++) {
				double value;
				in >> value;
				speed_model[p] = value;
			}
			edges[e].speed_model = speed_model;
		}

		if (demand == 0)
			n_edgesNonRequired++;
		else
			n_edgesRequired++;


	}
}

