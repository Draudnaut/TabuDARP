#pragma once
#include "parameter.h"
#include "solution.h"
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
#include "record_move.h"
#include <cstdlib>
#include <cstdio>
#include <mutex>
#include <thread>
struct TourBlock
{
	std::vector<Tour> Tourset;
};
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
struct vnsSwapNeighborhoodStructure
{
	int tour1;
	std::vector<int> requestTour1Selected;
	int tour2;
	std::vector<int> requestTour2Selected;
};
struct vnsChainNeighborhoodStructure
{
	int tour1;
	int tour2;
	std::vector<int> requestSeq;
};
std::vector<neighbor_structure> getNeighbors(solution s, Data &d, int iteration, Record_move &rm, Parameter &p);
std::vector<neighbor_structure> getNeighbors_ex(solution s,Data &d,int iteration,Record_move &rm,Parameter &p);
solution BigRemove(solution s,Data &d,Parameter &p);
bool tabulist_contains(std::vector<int> &tabuList,solution &s);
void TabuSearch(solution s, Parameter p,Data &d,Record_move &rm,int indicator);
void VariableNeighborSearch(solution s, Parameter p,Data &d);
void paraNeighborSearch(solution s, Parameter p);
solution present_solution(solution Base, neighbor_structure change, Parameter &p, Data &d);
void paraTabuSearch(solution s, Parameter p, Data &d, Record_move &rm);
std::vector<vnsSwapNeighborhoodStructure> getSwapNeighbor(solution s);
std::vector<vnsSwapNeighborhoodStructure> getSwapNeighbor_ex(solution s);
solution presentSwapSolution(solution Base, vnsSwapNeighborhoodStructure neighbor_present, Parameter &p, Data &d);
std::vector<vnsChainNeighborhoodStructure> getChainNeighbor(solution s);
std::vector<vnsChainNeighborhoodStructure> getChainNeighbor_ex(solution s);
solution chainNeighborPresent(solution Base, vnsChainNeighborhoodStructure neighbor_present, Parameter &p, Data &d);
