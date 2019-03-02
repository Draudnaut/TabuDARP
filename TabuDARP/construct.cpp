#include "pch.h"
#include "construct.h"

solution construct(Data & d,Memory &m)
{
	solution s;
	for (int i = 1; i < d.get_vertex_number()/2; i++)
	{
		int k = rand() % d.get_vehicle_number();
		s.get_Tour(k).insert_node(s.get_Tour(k).get_length(), i,d);
		s.get_Tour(k).insert_node(s.get_Tour(k).get_length(), i+d.get_vertex_number()/2, d);
		if (s.get_Tour(k).get_length() == 2) {
			s.set_length(s.get_length() + 1);
		}
		m.update_request_memory(k, i);
	}
	return s;
}
