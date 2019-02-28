#pragma once
#include "pch.h"
#include "tool.h"
#include "Data.h"
class Tour
{
private:
	int nodelist[200];
	double depart[200];
	double arrive[200];
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
	void update(Data &d);
	void delete_node(int index);
	void insert_node(int index, int node,Data &d);
	void output();
};