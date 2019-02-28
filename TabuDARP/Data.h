#pragma once
#include "pch.h"
class Data
{
private:
	Point point[2005];
	int vehicle_number, vertex_count, maximum_distance_vehicle, capacity, ridetime;
	char dataset[200];
public:
	Data(const char* path);
	int get_vehicle_number() { return vehicle_number; };
	int get_vertex_number() { return vertex_count; };
	int get_maximum_distance_vehicle() { return maximum_distance_vehicle; };
	int get_capacity() { return capacity; };
	int get_ridetime() { return ridetime; };
	Point get_point(int index) { return point[index]; }
};