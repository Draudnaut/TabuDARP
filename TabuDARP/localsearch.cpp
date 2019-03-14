#include "pch.h"
#include "localsearch.h"

std::vector<neighbor_structure> getNeighbors(solution s,Data &d,int iteration,Record_move &rm,Parameter &p)
{
	std::vector<neighbor_structure> ans;
	int length = s.get_length();
	for (int i = 0; i < length; i++)
	{
		neighbor_structure temp_neighbor;
		for (int j = 0; j < length; j++)
		{
			if (i == j) continue;
			temp_neighbor.tour1 = i;
			temp_neighbor.tour2 = j;
			std::vector<int> nodelist = s.get_Tour(i).get_nodelist();
			for (int k = 0; k < nodelist.size(); k++)
			{
				if (PickupOrDelivery(d.get_vertex_number(),nodelist[k])==pickup && rm.isOkToOperate(nodelist[k],i,iteration,p.get_theta()) && s.get_Tour(i).get_length()>0)
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
	/*TODO:reimplementation of big remove*/
	for (int i = 0; i < s.get_length(); i++)
	{
		Tour t = s.get_Tour(i);
		Tour t2 = t;
		int requestInTour[200];
		memset(requestInTour, -1, sizeof(requestInTour));
		int index = -1;
		std::vector<int> nodelist = t.get_nodelist();
		std::vector<int> requestList;
		for (auto & node : nodelist) if (PickupOrDelivery(d.get_vertex_number(), node) == pickup) requestList.push_back(node);
		std::random_shuffle(requestList.begin(), requestList.end());
		double cost = t.get_cost(p, d);
		for (auto & request : requestList)
		{
			int pickupToRealloc = request;
			int deliveryToRealloc = request + d.get_vertex_number() / 2;
			t2.delete_node(deliveryToRealloc);
			t2.delete_node(pickupToRealloc);
			int len = t2.get_length();
			int indexPickupToInsert = -1;
			int indexDeliveryToInsert = -1;
			for (int j = len; j > 0; j--)
			{
				t2.insert_node(j,deliveryToRealloc,d);
				for (int k = 0; k < j; k++)
				{
					t2.insert_node(k, pickupToRealloc, d);
					t2.update(d);
					if (t2.get_cost(p, d) < cost)
					{
						cost = t2.get_cost(p, d);
						indexPickupToInsert = k;
						indexDeliveryToInsert = j;
					}
					t2.delete_node(pickupToRealloc);
				}
				t2.delete_node(deliveryToRealloc);
			}
			if (indexPickupToInsert != -1 and indexDeliveryToInsert != -1)
			{
				t.delete_node(pickupToRealloc);
				t.delete_node(deliveryToRealloc);
				t.insert_node(indexDeliveryToInsert, deliveryToRealloc, d);
				t.insert_node(indexPickupToInsert, pickupToRealloc, d);
				t.update(d);
			}
		}
	}
	s.update(p, d);
	return s;
}

bool tabulist_contains(std::vector<solution> &tabuList, solution &s)
{
	for (auto &item : tabuList)
	{
		if (s == item) return true;
	}
	return false;
}

void TabuSearch(solution s, Parameter p, Data &d, Record_move &rm)
{
	FILE *f;
	f = fopen("E:/output/Tabu_Result.txt", "w+");
	if (f == NULL)
	{
		printf("file open failed.\n");
		exit(-1);
	}
	int current_iterator = 0;
	int find_feasible = -1;
	clock_t end, start;
	s.update(p, d);
	solution sBest = s;
	solution bestCandidate = s;
	solution feasibleSolution;
	sBest.update(p, d);
	bestCandidate.update(p, d);
	static std::vector<solution> tabuList;
	//tabuList.reserve(1000);
	tabuList.push_back(sBest);
	const int max_tabu_size = 20;
	start = end = clock();
	int tabu_length = tabuList.size();
	//s.output("123");
	//puts("--------------");
	while (end - start < 20*60 * CLOCKS_PER_SEC)
	{
		++current_iterator;
		fprintf(f, "%d %.4lf\n", current_iterator, sBest.get_cost(p, d));
		printf("-------------------------------------iterator : %d----------------------------------------------\n", current_iterator);
		if (current_iterator % p.get_ke() == 0) 
		{ 
			bestCandidate = BigRemove(bestCandidate, d, p); 
			bestCandidate.update(p, d); 
			printf("Big update completed. cost : %.4lf\n", bestCandidate.get_cost(p, d));
		}
		std::vector<neighbor_structure> sNeighborhood = getNeighbors(bestCandidate, d,current_iterator,rm,p);
		solution tbCandidate = bestCandidate;
		neighbor_structure record_bestOperation;
		//printf("vector neighborhood list size : %d.\n", sNeighborhood.size());
		for (auto sCandidate_representation : sNeighborhood)
		{
			solution sCandidate = present_solution(bestCandidate,sCandidate_representation,p,d);
			sCandidate.update(p, d);
			if ((tabulist_contains(tabuList, sCandidate)==false and sCandidate.get_cost(p,d) < tbCandidate.get_cost(p,d)))
			{
				//puts("solution update");
				tbCandidate = sCandidate;
				//tbCandidate.get_Tour(sCandidate_representation.tour1).output();
				//puts("");
				//tbCandidate.get_Tour(sCandidate_representation.tour2).output();
				//puts("");
				record_bestOperation = sCandidate_representation;
			}
			if (sCandidate.get_feasibility() == true)
			{
				printf("find a feasible solution\n");
				if (find_feasible == -1)
				{
					find_feasible = 1;
					feasibleSolution = sCandidate;
				}
				else {
					if (feasibleSolution.get_cost(p, d) > sCandidate.get_cost(p, d))
					{
						feasibleSolution = sCandidate;
					}
				}
			}
		}
		if (tbCandidate.get_cost(p, d) < bestCandidate.get_cost(p, d)) 
		{ 
			bestCandidate = tbCandidate; 
		}
		if (bestCandidate.get_cost(p,d) < sBest.get_cost(p,d))
		{
			sBest = bestCandidate;
			printf("update sBest,sBest cost = %.4lf\n",sBest.get_cost(p,d));
			rm.setRecord(record_bestOperation.request, record_bestOperation.tour1, current_iterator);
			
		}
		tabuList.push_back(bestCandidate);
		if (tabuList.size() > 400)
		{
			tabuList.clear();
		}
		if (current_iterator % 5 == 0) {
			//checkpoint;
			sBest.update(p, d);
			printf("--------------current solution-----------------\n");
			if (sBest.get_feasibility() == false) {
				printf("infeasible solution cost : %.4lf , hard_cost = %.4lf\n", sBest.get_cost(p,d),sBest.hard_cost());
				printf("TabuList size : %d\n", tabuList.size());
				printf("sBest tourlist every len : [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d].\n",
					sBest.get_Tour(0).get_length(),
					sBest.get_Tour(1).get_length(),
					sBest.get_Tour(2).get_length(),
					sBest.get_Tour(3).get_length(),
					sBest.get_Tour(4).get_length(),
					sBest.get_Tour(5).get_length(),
					sBest.get_Tour(6).get_length(),
					sBest.get_Tour(7).get_length(),
					sBest.get_Tour(8).get_length(),
					sBest.get_Tour(9).get_length(),
					sBest.get_Tour(10).get_length(),
					sBest.get_Tour(11).get_length(),
					sBest.get_Tour(12).get_length()
					);
				sBest.checkFeasibility(d);
				//sBest.output("123");
			}
			else {
				printf("feasible solution cost : %.4lf\n", sBest.get_cost(p, d));
			}
		}
		p.update(sBest.get_feasibility());
		end = clock();
	}
	fclose(f);
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
	/*when insertion takes place , we should find a best position to lower the whole cost*/
	int length_tour2 = tour2.get_length();
	int pickupToInsert = change.request;
	int deliveryToInsert = change.request + d.get_vertex_number() / 2;
	int indexPickupInsertTo = -1;
	int indexDeliveryInsertTo = -1;
	double costTour2 = 999999999;
	for (int i = length_tour2; i >= 0; i--)
	{
		tour2.insert_node(i, deliveryToInsert, d);
		for (int j = 0; j < i; j++)
		{
			tour2.insert_node(j, pickupToInsert, d);
			tour2.update(d);
			if (tour2.get_cost(p, d) < costTour2)
			{
				costTour2 = tour2.get_cost(p, d);
				indexDeliveryInsertTo = i;
				indexPickupInsertTo = j;
			}
			
			tour2.delete_node(pickupToInsert);
		}
		tour2.delete_node(deliveryToInsert);
	}
	if (indexDeliveryInsertTo != -1 and indexPickupInsertTo != -1)
	{
		tour2.insert_node(indexDeliveryInsertTo,deliveryToInsert,d);
		tour2.insert_node(indexPickupInsertTo, pickupToInsert, d);
		tour1.update(d);
		tour2.update(d);
		//printf("after changed cost: \n\ttour1 : %.4lf\n\ttour2 : %.4lf\n", tour1.get_cost(p, d), tour2.get_cost(p, d));
		Base.set_tourlist(tour1, change.tour1);
		Base.set_tourlist(tour2, change.tour2);
		Base.update(p, d);
	}
	return Base;
}