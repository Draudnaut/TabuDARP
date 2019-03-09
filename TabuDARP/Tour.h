#pragma once
#include "pch.h"
#include "tool.h"
#include "Data.h"
#include "parameter.h"
#include "tool.h"
#include "vector"
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
	double get_cost(Parameter &p,Data &d);
	void set_node(int index, int node);
	int get_length();
	void update(Data &d);
	void delete_node(int node);
	void insert_node(int index, int node,Data &d);
	void output();
	double violation_quality(Data &p);
	double violation_duration(Data &p);
	double violation_window(Data &p);
	double violation_ridetime(Data &p);
	bool operator == (const Tour &t) const;
	double hard_cost();
	std::vector<int> get_nodelist();
	void checkFeasibility(Data &d);
};