#include "pch.h"
#include "localsearch.h"


solution getNeighbors(solution s,Memory &m)
{
	
	return solution();
}

solution BigRemove(solution s)
{
	return solution();
}

void TabuSearch(solution s, Parameter p,Memory &m,Data &d)
{
	int current_iterator = -1;
	clock_t end, start;
	start = end = clock();
	solution sBest = s;
	solution bestCandidate = s;
	std::priority_queue<solution, std::vector<solution>,std::less<solution>> tabuList;
	tabuList.push(s);
	while (end - start < 60 * CLOCKS_PER_SEC)
	{
		++current_iterator;
		if (current_iterator == p.get_ke())  s = BigRemove(s);
		int len = sBest.get_length();
		/*choose two distinct tours to remove and reinsertion*/
		int tour1 = rand() % len;
		int tour2 = rand() % len;
		while (tour1 == tour2) tour2 = rand() % len;
		Tour t1 = sBest.get_Tour(tour1);
		Tour t2 = sBest.get_Tour(tour2);
		std::vector<int> nodeInTour1(t1.get_nodelist());
		std::vector<int> requestInTour1;
		for (auto a : nodeInTour1)
		{
			if (PickupOrDelivery(d.get_capacity(), a) == pickup) {
				requestInTour1.push_back(a);
			}
		}
		int request_remove = rand() % requestInTour1.size();
		
		int pickup_remove = requestInTour1[request_remove];
		int delivery_remove = pickup_remove + d.get_vertex_number() / 2;
		t1.delete_node(pickup_remove);
		t1.delete_node(delivery_remove);
		t2.insert_node(t2.get_length(), pickup_remove, d);
		t2.insert_node(t2.get_length(), delivery_remove, d);
		m.updateRecordOfRequestMove(pickup_remove, current_iterator);
		m.update_request_memory(tour2, pickup_remove);
		/*choose whether to update tour*/
		t1.update(d);
		t2.update(d);
		if (t1.get_cost(p, d) + t2.get_cost(p, d) < sBest.get_Tour(tour1).get_cost(p,d) + sBest.get_Tour(tour2).get_cost(p,d)) {
			/*update cost*/
			sBest.set_tourlist(t1, tour1);
			sBest.set_tourlist(t2, tour2);
		}

	}
}

void VariableNeighborSearch(solution s, Parameter p)
{
}

void paraNeighborSearch(solution s, Parameter p)
{
}

