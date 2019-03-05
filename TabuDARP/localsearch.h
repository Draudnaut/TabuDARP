#pragma once
#include "parameter.h"
#include "solution.h"
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
#include "Memory.h"
struct neighbor_structure
{
	int tour1;
	int tour2;
	int request;
};
std::vector<neighbor_structure> getNeighbors(solution s,Memory &m, Data &d);
solution BigRemove(solution s,Data &d,Parameter &p);
bool tabulist_contains(std::vector<int> &tabuList,solution &s);
void TabuSearch(solution s, Parameter p,Memory &m,Data &d);
void VariableNeighborSearch(solution s, Parameter p);
void paraNeighborSearch(solution s, Parameter p);
solution present_solution(solution Base, neighbor_structure change, Parameter &p, Data &d);