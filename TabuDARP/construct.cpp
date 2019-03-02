#include "pch.h"
#include "construct.h"

solution construct_Tabu(Data & d,Memory &m)
{
	solution s;
	int current_iterator = -1;
	for (int i = 1; i < d.get_vertex_number()/2; i++)
	{
		++current_iterator;
		int k = rand() % d.get_vehicle_number();
		s.get_Tour(k).insert_node(s.get_Tour(k).get_length(), i,d);
		s.get_Tour(k).insert_node(s.get_Tour(k).get_length(), i+d.get_vertex_number()/2, d);
		if (s.get_Tour(k).get_length() == 2) {
			s.set_length(s.get_length() + 1);
		}
		m.update_request_memory(k, i);
		m.updateRecordOfRequestMove(i, current_iterator);
	}
	return s;
}
