#pragma once
#include "pch.h"
class Data
{
private:
	Point point[2005];
	int vehicle_number, vertex_count, duration, capacity, ridetime;
	char dataset[200];
	std::set<std::pair<int, int>> infeasibleEdge;
public:
	Data(const char* path);
	int get_vehicle_number() { return vehicle_number; };
	int get_vertex_number() { return vertex_count; };
	int get_maximum_distance_vehicle() { return duration; };
	int get_capacity() { return capacity; };
	int get_ridetime() { return ridetime; };
	Point get_point(int index) { return point[index]; }
	void insertInfeasibleEdge(int i, int j);
	bool EdgeFeasiblity(int i, int j);
	std::vector<Point> pointArray();
};