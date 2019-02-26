#pragma once
#include "pch.h"
class Tour
{
private:
	int nodelist[200];
	double time_stamp[200];
	int weight[200];
	double distance[200];
	bool feasibility;
	int len;
public:
	Tour();
	Tour(const Tour &t);
	bool get_feasibility();
	double get_cost();
	void set_node(int index, int node);
	int get_length();
	void update();
	void delete_node(int index);
	void insert_node(int index, int node);
};