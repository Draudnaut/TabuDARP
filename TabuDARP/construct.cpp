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
	std::sort(PointArray.begin(), PointArray.end(), cmp_vns);
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
		
		index++;
	}
	return s;
}

solution construct_Para(Data & d)
{
	return solution();
}

double vnsCriterion1(Tour & t, Point a)
{
	return 0.0;
}

double vnsCriterion2(Tour & t, Point a)
{
	return 0.0;
}

double vnsCriterion3(Tour & t, Point a)
{
	return 0.0;
}

double vnsCriterion4(Tour & t, Point a)
{
	return 0.0;
}
