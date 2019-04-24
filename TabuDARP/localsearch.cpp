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

std::vector<neighbor_structure> getNeighbors_ex(solution s, Data & d, int iteration, Record_move & rm, Parameter & p)
{
	std::vector<std::pair<int,Tour>> tourList;
	for (int i = 0; i < s.get_length(); i++)
	{
		tourList.push_back(std::make_pair(i,s.get_Tour(i)));
	}
	std::sort(tourList.begin(), 
			tourList.end(), 
			[&](std::pair<int, Tour> &p1, std::pair<int, Tour> &p2)->int 
			{
				return p1.second.get_cost(p, d) > p2.second.get_cost(p, d); 
			}
	);
	/*checkPoint
	for (auto Candidate : tourList)
	{
		printf("tour id : %d , cost : %.4lf\n", Candidate.first, Candidate.second.get_cost(p, d));
	}*/
	int tourid1 = tourList.begin()->first;
	int tourid2 = (tourList.end()-1)->first;
	std::vector<int> nodelistTour1(tourList[0].second.get_nodelist()), 
					 nodelistTour2((tourList.end()-1)->second.get_nodelist());
	std::vector<neighbor_structure> neighbors;
	std::vector<int> requestlistTour1;
	for (auto node : nodelistTour1)
	{
		if (PickupOrDelivery(d.get_vertex_number(), node) == pickup)
		{
			requestlistTour1.push_back(node);
		}
	}
	neighbor_structure nei;
	nei.tour1 = tourid1;
	nei.tour2 = tourid2;
	for (auto request : requestlistTour1)
	{
		nei.request = request;
		neighbors.push_back(nei);
	}
	return neighbors;
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

void TabuSearch(solution s, Parameter p, Data &d, Record_move &rm,int indicator)
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
	tabuList.push_back(sBest);
	const int max_tabu_size = 20;
	start = end = clock();
	int tabu_length = tabuList.size();
	int countEvaluation = 0;
	while (end - start < 30*60 * CLOCKS_PER_SEC)
	{
		++current_iterator;
		fprintf(f, "%d %.4lf\n", countEvaluation, sBest.get_cost(p, d));
		if (current_iterator % p.get_ke() == 0) 
		{ 
			bestCandidate = BigRemove(bestCandidate, d, p); 
			bestCandidate.update(p, d); 
			printf("Big update completed. cost : %.4lf\n", bestCandidate.get_cost(p, d));
			if (bestCandidate.get_feasibility() == true) find_feasible = 1,printf("find feasible. Evaluation %d\n", countEvaluation);
		}
		std::vector<neighbor_structure> sNeighborhood;
		if (indicator == 1) sNeighborhood = getNeighbors_ex(bestCandidate, d,current_iterator,rm,p);
		else sNeighborhood = getNeighbors(bestCandidate, d, current_iterator, rm, p);
		solution tbCandidate = bestCandidate;
		neighbor_structure record_bestOperation;
		for (auto sCandidate_representation : sNeighborhood)
		{
			countEvaluation++;
			solution sCandidate = present_solution(bestCandidate,sCandidate_representation,p,d);
			sCandidate.update(p, d);
			if ((tabulist_contains(tabuList, sCandidate)==false and sCandidate.get_cost(p,d) < tbCandidate.get_cost(p,d)))
			{
				tbCandidate = sCandidate;
				record_bestOperation = sCandidate_representation;
			}
			if (sCandidate.get_feasibility() == true)
			{
				if (find_feasible == -1)
				{
					find_feasible = 1;
					feasibleSolution = sCandidate;
					printf("find feasible. Evaluation %d\n", countEvaluation);
				}
				else 
				{
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
			printf("Evaluation %d ,update sBest,sBest cost = %.4lf\n",countEvaluation,sBest.get_cost(p,d));
			rm.setRecord(record_bestOperation.request, record_bestOperation.tour1, current_iterator);
			
		}
		tabuList.push_back(bestCandidate);
		if (tabuList.size() > 400)
		{
			tabuList.clear();
		}
		if (countEvaluation % 1000 == 0) {
			//checkpoint;
			sBest.update(p, d);
			printf("count on evaluation : %d\n",countEvaluation);
			if (sBest.get_feasibility() == false) {
				printf("infeasible solution cost : %.4lf , hard_cost = %.4lf\n", sBest.get_cost(p,d),sBest.hard_cost());
			}
			else {
				printf("feasible solution cost : %.4lf\n", sBest.get_cost(p, d));
				sBest.output("123");
			}
		}
		p.update(sBest.get_feasibility());
		end = clock();
	}
	fclose(f);
}

void VariableNeighborSearch(solution s, Parameter &p, Data &d,int indicate)
{
	int neighboring_k = 1;
	clock_t start, end;
	start = end = clock();
	solution sBest = s;
	int iter = 0;
	double bCost = sBest.get_cost(p,d);
	while (end - start < 5*60 * CLOCKS_PER_SEC)
	{
		// shaking
		printf("current iteration %d , cost %.4lf\n", ++iter, bCost);
		solution s_ = shake(s, neighboring_k,d,p,indicate);
		s_.update(p, d);
		p.update(s_.get_feasibility());
		//local search
		double prand = ((double)rand() / RAND_MAX);
		solution s__;
		if (s_.hard_cost() < 1.02*s.hard_cost() or prand < 0.01) {
			s__ = vnsLocalSearch(s_);
		}
		else{
			s__ = s;
		}
		//move or not
		if (s__.get_cost(p, d) < s.get_cost(p, d)){
			if (s__.hard_cost() >= 1.05*s.hard_cost()){
				s__ = vnsLocalSearch(s__);
			}
			s = s__;
			if (s.get_cost(p, d) < bCost) bCost = s.get_cost(p, d);
			neighboring_k = 0;
		}
		if (s__.get_feasibility() == true and s__.get_cost(p, d) < sBest.get_cost(p, d)){
			printf("find feasible , iter : %d\n", iter);
			sBest = s__;
		}
		//modify k
		neighboring_k = (neighboring_k % 13) + 1;
		end = clock();
		//p.output();
		s.update(p,d);
	}
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

void paraTabuSearch(solution s, Parameter p, Data & d, Record_move & rm)
{
}

solution getSwapNeighbor(solution s,Data &d,Parameter &p,int indicate)
{
	int len = s.get_length();
	int touri = rand() % len;
	int tourj = rand() % len;
	if (indicate == 1)
	{
		std::vector<std::pair<int, Tour>> tourList;
		for (int i = 0; i < len; i++)
		{
			tourList.push_back(std::make_pair(i, s.get_Tour(i)));
		}
		std::sort(tourList.begin(), tourList.end(),
			[&](std::pair<int, Tour> &x, std::pair<int, Tour> &y)->int 
		    {
			return x.second.get_cost(p, d) > y.second.get_cost(p, d);
		    }
		);
		touri = tourList.begin()->first;
		tourj = (tourList.end() - 1)->first;
	}
	while (tourj == touri) tourj = rand() % len;
	int length_tour1 = s.get_Tour(touri).get_length();
	int length_tour2 = s.get_Tour(tourj).get_length();
	/*choose sequence index from Tour1 and tour2*/
	int index_tour1_begin, index_tour1_end, index_tour2_begin, index_tour2_end;
	index_tour1_begin = rand() % length_tour1;
	index_tour1_end = rand() % length_tour1;
	if (index_tour1_end <= index_tour1_begin) std::swap(index_tour1_begin,index_tour1_end);
	index_tour2_begin = rand() % length_tour2;
	index_tour2_end = rand() % length_tour2;
	if (index_tour2_end <= index_tour2_begin) std::swap(index_tour2_begin,index_tour2_end);
	std::vector<int> requestTour1, requestTour2;
	/*get request sequence*/
	int i = index_tour1_begin;
	while (i <= index_tour1_end)
	{
		int node = s.get_Tour(touri).get_node(index_tour1_begin);
		int flag = 0;
		for (auto nodeExist : requestTour1)
		{
			if (node == nodeExist or isCorrespondPD(d.get_vertex_number(), node, nodeExist))
			{
				flag = 1;
			}
		}
		if (flag == 0)
		{
			if (PickupOrDelivery(d.get_vertex_number(), node) == pickup)
			{
				requestTour1.push_back(node);
			}
			else
			{
				requestTour1.push_back(node - d.get_vertex_number() / 2);
			}
		}
		i++;
	}
	i = index_tour2_begin;
	while (i <= index_tour2_end)
	{
		int node = s.get_Tour(tourj).get_node(index_tour2_begin);
		int flag = 0;
		for (auto nodeExist : requestTour2)
		{
			if (node == nodeExist or isCorrespondPD(d.get_vertex_number(), node, nodeExist))
			{
				flag = 1;
			}
		}
		if (flag == 0)
		{
			if (PickupOrDelivery(d.get_vertex_number(), node) == pickup)
			{
				requestTour2.push_back(node);
			}
			else
			{
				requestTour2.push_back(node - d.get_vertex_number() / 2);
			}
		}
		i++;
	}
	/*delete request sequence*/
	Tour t1 = s.get_Tour(touri);
	Tour t2 = s.get_Tour(tourj);
	for (auto node : requestTour1)
	{
		t1.delete_node(node);
		t1.delete_node(node + (d.get_vertex_number() / 2));
	}
	for (auto node : requestTour2)
	{
		t2.delete_node(node);
		t2.delete_node(node + (d.get_vertex_number() / 2));
	}
	//t1.update(d);
	//t2.update(d);
	/*
	   reinsert request
	   sequence1 to tour2
	   sequence2 to tour1
	*/
	t1 = vnsSwapUpdateTour(t1, requestTour2,d,p);
	t2 = vnsSwapUpdateTour(t2, requestTour1,d,p);
	t1.update(d);
	t2.update(d);
	s.set_tourlist(t1, touri);
	s.set_tourlist(t2, tourj);
	return s;
}

solution getChainNeighbor(solution s, Data &d, Parameter &p, int indicate)
{
	return solution();
}

solution shake(solution s, int k,Data &d,Parameter &p,int indicate)
{
	s = getSwapNeighbor(s, d, p,indicate);
	return s;
}

solution vnsLocalSearch(solution s)
{
	return s;
}

Tour vnsSwapUpdateTour(Tour t, std::vector<int>& requestList,Data &d,Parameter &p)
{
	for (auto request : requestList)
	{
		double cost;
		int pickupToInsert = request;
		int deliveryToInsert = request + d.get_vertex_number() / 2;
		int index_pickupToinsert = -1;
		int index_deliveryToInsert = -1;
		int length = t.get_length();
		for (int i = 0; i <= length; i++)
		{
			t.insert_node(i, deliveryToInsert, d);
			for (int j = 0; j <= i; j++)
			{
				t.insert_node(j, pickupToInsert, d);
				double tcost = t.get_cost(p, d);
				if (index_pickupToinsert == -1 and index_deliveryToInsert == -1)
				{
					index_deliveryToInsert = i;
					index_pickupToinsert = j;
					cost = tcost;
				}
				else
				{
					if (tcost < cost)
					{
						index_pickupToinsert = j;
						index_deliveryToInsert = i;
						cost = tcost;
					}
				}
				t.delete_node(pickupToInsert);
			}
			t.delete_node(deliveryToInsert);
		}
		t.insert_node(index_deliveryToInsert, deliveryToInsert, d);
		t.insert_node(index_pickupToinsert, pickupToInsert, d);
	}
	return t;
}
