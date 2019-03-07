#include "pch.h"
#include "localsearch.h"

std::vector<neighbor_structure> getNeighbors(solution s, Memory & m,Data &d)
{
	std::vector<neighbor_structure> ans;
	int length = s.get_length();
	for (int i = 0; i < length; i++)
	{
		if (ans.size() == 400) break;
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

solution BigRemove(solution s,Data &d,Parameter &p)
{
	puts("Big operator");
	int length = s.get_length();
	for (int i = 0; i < length; i++)
	{
		Tour change = s.get_Tour(i);
		std::vector<int> nodelist = change.get_nodelist();
		for (int j=0;j<nodelist.size();j++)
		{
			puts("pick up small operator");
			if (PickupOrDelivery(d.get_vertex_number(),nodelist[j]) == pickup)
			{
				Tour tChange = change;
				double costChange = change.get_cost(p, d);
				tChange.delete_node(nodelist[j]);
				int correspoondDelivery = nodelist[j] + d.get_vertex_number() / 2;
				int indexPickupToInsert = -1;
				for (int k = 0; k < nodelist.size()&&nodelist[k]!=correspoondDelivery; k++)
				{
					tChange.insert_node(k, nodelist[j], d);
					double cost = tChange.get_cost(p, d);
					if (cost < costChange)
					{
						indexPickupToInsert = k;
						costChange = cost;
					}
					tChange.delete_node(nodelist[j]);
				}
				if (indexPickupToInsert != -1)
				{
					change.delete_node(nodelist[j]);
					change.insert_node(indexPickupToInsert, nodelist[j], d);
					s.set_tourlist(change, j);
				}
			}
			else
			{
				puts("move delivery");
				Tour tChange = change;
				double costChange = tChange.get_cost(p, d);
				int correspondPickup = nodelist[j] - d.get_vertex_number() / 2;
				int indexCorrespondPickup = -1;
				for (int k = 0; k < nodelist.size(); k++)
				{
					if (nodelist[k]==correspondPickup)
					{
						indexCorrespondPickup = j;
						break;
					}
				}
				tChange.delete_node(nodelist[i]);
				int indexToInsertDelivery = -1;
				for (int k = indexCorrespondPickup + 1; k < nodelist.size(); k++)
				{
					tChange.insert_node(k, nodelist[i], d);
					double cost = tChange.get_cost(p, d);
					if (cost < costChange)
					{
						costChange = cost;
						indexToInsertDelivery = k;
					}
				}
				if (indexToInsertDelivery != -1)
				{
					change.delete_node(nodelist[j]);
					change.insert_node(indexToInsertDelivery, nodelist[j], d);
					s.set_tourlist(change, j);
				}
			}
		}
	}
	return s;
}

bool tabulist_contains(std::vector<solution> &tabuList, solution &s)
{
	for (auto item : tabuList)
	{
		if (s == item) return true;
	}
	return false;
}

void TabuSearch(solution s, Parameter p,Memory &m,Data &d)
{
	int current_iterator = 0;
	clock_t end, start;
	solution sBest = s;
	solution bestCandidate = s;
	sBest.update(p, d);
	bestCandidate.update(p, d);
	static std::vector<solution> tabuList;
	//tabuList.reserve(1000);
	tabuList.push_back(sBest);
	const int max_tabu_size = 20;
	start = end = clock();
	int tabu_length = tabuList.size();
	while (end - start < 2*60 * CLOCKS_PER_SEC)
	{
		++current_iterator;
		printf("iterator : %d\n", current_iterator);
		if (current_iterator % p.get_ke() == 0) 
		{ 
			bestCandidate = BigRemove(bestCandidate, d, p); 
			bestCandidate.update(p, d); 
			printf("Big update completed. cost : %.4lf\n", bestCandidate.get_cost(p, d));
		}
		puts("small update");
		std::vector<neighbor_structure> sNeighborhood = getNeighbors(bestCandidate, m, d);
		solution tbCandidate = bestCandidate;
		for (auto sCandidate_representation : sNeighborhood)
		{
			solution sCandidate = present_solution(bestCandidate,sCandidate_representation,p,d);
			sCandidate.update(p, d);	
			if ((not tabulist_contains(tabuList, sCandidate) and sCandidate.get_cost(p,d) < tbCandidate.get_cost(p,d)))
			{
				//printf("update sCandidate , cost = %.4lf\n", sCandidate.get_cost(p,d));
				tbCandidate = sCandidate;
			}
		}
		if (tbCandidate.get_cost(p, d) < bestCandidate.get_cost(p, d)) bestCandidate = tbCandidate;
		if (bestCandidate.get_cost(p,d) < sBest.get_cost(p,d))
		{
			sBest = bestCandidate;
			printf("update sBest,sBest cost = %.4lf\n",sBest.get_cost(p,d));
		}
		tabuList.push_back(bestCandidate);
		//if (tabuList.size() > tabu_length) { tabu_length = tabuList.size(); printf("tabu list length : %d\n", tabuList.size()); }
		//tabuList.pop();
		//p.update(sBest.get_feasibility());
		if (current_iterator % 10 == 0) {
			//checkpoint;
			printf("--------------current solution-----------------\n");
			if (sBest.get_feasibility() == false) {
				printf("infeasible solution cost : %.4lf , hard_cost = %.4lf\n", sBest.get_cost(p,d),sBest.hard_cost());
				printf("TabuList size : %d\n", tabuList.size());
			}
			else {
				printf("feasible solution cost : %.4lf\n", sBest.get_cost(p, d));
			}
		}
		end = clock();
		//printf("iter : %d\n", current_iterator);
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