#include "pch.h"
#include "construct.h"

solution construct_Tabu(Data & d,Record_move &rm)
{
	solution s;
	int current_iterator = -1;
	for (int i = 1; i <= d.get_vertex_number()/2; i++)
	{
		++current_iterator;
		int k = rand() % d.get_vehicle_number();
		s.get_Tour(k).insert_node(s.get_Tour(k).get_length(), i,d);
		s.get_Tour(k).insert_node(s.get_Tour(k).get_length(), i+d.get_vertex_number()/2, d);
		if (s.get_Tour(k).get_length() == 2) {
			s.set_length(s.get_length() + 1);
		}
		rm.setRecord(i, k, 0);
	}
	return s;
}

solution construct_VNS(Data & d)
{
	std::vector<Point> PointArray = d.pointArray();
	std::vector<Point> requestArray(PointArray.begin()+1, PointArray.begin() + (PointArray.size() / 2));
	std::sort(requestArray.begin(), requestArray.end(), cmp_vns);
	solution s;
	s.set_length(d.get_vehicle_number());
	int vehicle_number = d.get_vehicle_number();
	int index = 0;
	for (int i = 0; i < vehicle_number; i++)
	{
		s.get_Tour(i).insert_node(s.get_Tour(i).get_length(), requestArray[index].id, d);
		s.get_Tour(i).insert_node(s.get_Tour(i).get_length(), requestArray[index].id + (d.get_vertex_number() / 2), d);
		index++;
	}
	while (index < requestArray.size())
	{
		/*choose one criterion and judge to move*/
		int k = rand() % 4;
		int tour_insert = -1;
		double maxCost = 9999999999;
		if (k == 0) 
		{
			for (int i = 0; i < vehicle_number; i++)
			{
				double dist = vnsCriterion1(s.get_Tour(i), requestArray[index], d);
				if (dist < maxCost)
				{
					tour_insert = i;
					maxCost = dist;
				}
			}
		}
		else if (k == 1)
		{
			for (int i = 0; i < vehicle_number; i++)
			{
				double dist = vnsCriterion2(s.get_Tour(i), d.get_point(requestArray[index].id + d.get_vertex_number() / 2), d);
				if (dist < maxCost)
				{
					tour_insert = i;
					maxCost = dist;
				}
			}
		}
		else if (k == 2)
		{
			for (int i = 0; i < vehicle_number; i++)
			{
				double dist = vnsCriterion3(s.get_Tour(i), requestArray[index], d);
				if (dist < maxCost)
				{
					tour_insert = i;
					maxCost = dist;
				}
			}
		}
		else
		{
			for (int i = 0; i < vehicle_number; i++)
			{
				double dist = vnsCriterion4(s.get_Tour(i), d.get_point(requestArray[index].id + d.get_vertex_number() / 2), d);
				if (dist < maxCost)
				{
					tour_insert = i;
					maxCost = dist;
				}
			}
		}
		s.get_Tour(tour_insert).insert_node(s.get_Tour(tour_insert).get_length(), requestArray[index].id, d);
		s.get_Tour(tour_insert).insert_node(s.get_Tour(tour_insert).get_length(), requestArray[index].id + d.get_vertex_number()/2, d);
		index++;
	}
	return s;
}

solution construct_Para(Data & d)
{
	return solution();
}

double vnsCriterion1(Tour & t, Point a,Data &d)
{
	/*origin origin*/
	std::vector<int> nodelist = t.get_nodelist();
	int lastPickup = *(nodelist.end() - 2);
	double ans = dis(d.get_point(lastPickup), a);
	return ans;
}

double vnsCriterion2(Tour & t, Point a,Data &d)
{
	/*origin destination*/
	std::vector<int> nodelist = t.get_nodelist();
	int lastpickup = *(nodelist.end() - 2);
	return dis(d.get_point(lastpickup),a);
}

double vnsCriterion3(Tour & t, Point a,Data &d)
{
	std::vector<int> nodelist = t.get_nodelist();
	int lastDelivery = *(nodelist.end() - 1);
	return dis(d.get_point(lastDelivery),a);
}

double vnsCriterion4(Tour & t, Point a,Data &d)
{
	std::vector<int> nodelist = t.get_nodelist();
	int lastDelivery = *(nodelist.end() - 1);
	return dis(d.get_point(lastDelivery),a);
}
