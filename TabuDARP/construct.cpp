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
	std::sort(PointArray.begin(), PointArray.end(), cmp_vns);
	solution s;
	s.set_length(d.get_vehicle_number());
	for (int i = 0; i < d.get_vehicle_number(); i++)
	{
		s.get_Tour(i).set_node(0, PointArray[i].id);
	}

	return s;
}

solution construct_Para(Data & d)
{
	return solution();
}



