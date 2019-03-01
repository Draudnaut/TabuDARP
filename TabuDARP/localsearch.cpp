#include "pch.h"
#include "localsearch.h"

void Tabu_search(solution & s, Parameter & p)
{
}

solution getNeighbors(solution s)
{
	return solution();
}

void TabuSearch(solution s, Parameter p)
{
	clock_t end, start;
	start = end = clock();
	solution sBest = s;
	solution bestCandidate = s;
	std::queue<solution, std::vector<solution>> tabuList;
	tabuList.push(s);
	while (end - start < 60 * CLOCKS_PER_SEC)
	{

	}
}

