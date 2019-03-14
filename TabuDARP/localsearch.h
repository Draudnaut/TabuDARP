#pragma once
#include "parameter.h"
#include "solution.h"
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
#include "record_move.h"
struct neighbor_structure
{
	/*
	parameter meaning:
	   from tour1 remove request to insert into tour 2
	*/
	int tour1;
	int tour2;
	int request;
};
std::vector<neighbor_structure> getNeighbors(solution s, Data &d, int iteration, Record_move &rm, Parameter &p);
solution BigRemove(solution s,Data &d,Parameter &p);
bool tabulist_contains(std::vector<int> &tabuList,solution &s);
void TabuSearch(solution s, Parameter p,Data &d,Record_move &rm);
void VariableNeighborSearch(solution s, Parameter p);
void paraNeighborSearch(solution s, Parameter p);
solution present_solution(solution Base, neighbor_structure change, Parameter &p, Data &d);