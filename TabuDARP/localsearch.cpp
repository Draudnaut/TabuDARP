#include "pch.h"
#include "localsearch.h"

std::vector<neighbor_structure> getNeighbors(solution s, Memory & m,Data &d)
{
	std::vector<neighbor_structure> ans;
	int length = s.get_length();
	for (int i = 0; i < length; i++)
	{
		neighbor_structure temp_neighbor;
		for (int j = i + 1; j < length; j++)
		{
			temp_neighbor.tour1 = i;
			temp_neighbor.tour2 = j;
			std::vector<int> nodelist = s.get_Tour(i).get_nodelist();
			for (int k = 0; k < nodelist.size(); k++)
			{
				if (PickupOrDelivery(d.get_vertex_number(),nodelist[k])==pickup)
				{
					temp_neighbor.request = nodelist[k];
					ans.push_back(temp_neighbor);
				}
			}
		}
	}
	return ans;
}

solution BigRemove(solution s)
{
	return s;
}

bool tabulist_contains(std::priority_queue<solution, std::vector<solution>, std::less<solution>> tabuList, solution &s)
{
	while (tabuList.empty() != true)
	{
		solution t = tabuList.top();
		if(t==s)
		{
			return true;
		}
		tabuList.pop();
	}
	return false;
}

void TabuSearch(solution s, Parameter p,Memory &m,Data &d)
{
	int current_iterator = -1;
	clock_t end, start;
	solution sBest = s;
	solution bestCandidate = s;
	sBest.update(p, d);
	bestCandidate.update(p, d);
	static std::priority_queue<solution, std::vector<solution>,std::less<solution>> tabuList;
	tabuList.push(sBest);
	const int max_tabu_size = 20;
	start = end = clock();
	while (end - start < 60 * CLOCKS_PER_SEC)
	{
		++current_iterator;
		//printf("iterator : %d\n", current_iterator);
		//if (current_iterator == p.get_ke())  bestCandidate = BigRemove(bestCandidate);
		std::vector<neighbor_structure> sNeighborhood;
		for (auto sCandidate_representation : sNeighborhood)
		{
			sNeighborhood = getNeighbors(tabuList.top(), m, d);
			solution sCandidate = present_solution(bestCandidate,sCandidate_representation,p,d);
			sCandidate.update(p, d);
			printf("sCandidate: \n\ttour1 : %.4lf\n\ttour2 : %.4lf\n", sCandidate.get_Tour(sCandidate_representation.tour1).get_cost(p, d),sCandidate.get_Tour(sCandidate_representation.tour2).get_cost(p, d));
			if ((not tabulist_contains(tabuList, sCandidate) and sCandidate.get_cost(p,d) < bestCandidate.get_cost(p,d)))
			{
				bestCandidate = sCandidate;
			}
		}
		if (bestCandidate.get_cost(p,d) < sBest.get_cost(p,d))
		{
			printf("update sBest,sBest cost = %.4lf\n",sBest.get_cost(p,d));
			sBest = bestCandidate;
		}
		tabuList.push(bestCandidate);
		tabuList.pop();
		//p.update(sBest.get_feasibility());
		if (current_iterator % 10 == 0) {
			//checkpoint;
			printf("--------------current solution-----------------\n");
			if (sBest.get_feasibility() == false) {
				printf("infeasible solution cost : %.4lf , hard_cost = %.4lf\n", sBest.get_cost(p,d),sBest.hard_cost());
			}
			else {
				printf("feasible solution cost : %.4lf\n", sBest.get_cost(p, d));
			}
		}
		end = clock();
	}
}

void VariableNeighborSearch(solution s, Parameter p)
{
}

void paraNeighborSearch(solution s, Parameter p)
{
}

solution present_solution(solution Base, neighbor_structure change,Parameter &p,Data &d)
{
	Tour tour1 = Base.get_Tour(change.tour1);
	Tour tour2 = Base.get_Tour(change.tour2);
	//printf("before changed cost: \n\ttour1 : %.4lf\n\ttour2 : %.4lf\n", tour1.get_cost(p, d), tour2.get_cost(p, d));
	std::vector<int> nodelist = tour1.get_nodelist();
	int index_pickup=-1, index_delivery=-1;
	for (int i = 0; i < nodelist.size(); i++)
	{
		if (nodelist[i] == change.request) index_pickup = i;
		if (nodelist[i] == change.request + d.get_vertex_number() / 2) index_delivery = i;
	}
	if (index_pickup == -1 || index_delivery == -1) {
		printf("ERROR in index pickup\n");
		printf("%d %d %d\n", change.tour1, change.tour2, change.request);
		exit(-1);
	}
	tour1.delete_node(change.request);
	tour1.delete_node(change.request + d.get_vertex_number() / 2);
	tour2.insert_node(tour2.get_length(), change.request, d);
	tour2.insert_node(tour2.get_length(), change.request + d.get_vertex_number() / 2, d);
	tour1.update(d);
	tour2.update(d);
	//printf("after changed cost: \n\ttour1 : %.4lf\n\ttour2 : %.4lf\n", tour1.get_cost(p, d), tour2.get_cost(p, d));
	Base.set_tourlist(tour1, change.tour1);
	Base.set_tourlist(tour2, change.tour2);
	Base.update(p, d);
	return Base;
}