#include "pch.h"
#include "construct.h"

solution construct(Data & d)
{
	solution s;
	for (int i = 1; i < d.get_vertex_number(); i++)
	{
		int k = rand() % d.get_vehicle_number();
		s.get_Tour(k).insert_node(s.get_Tour(k).get_length(), i);
		if (s.get_Tour(k).get_length() == 1) {
			puts("ADD");
			s.set_length(s.get_length() + 1);
		}
		printf("construct %d \n", k);
	}
	return s;
}
