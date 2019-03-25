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
		rm.setRecord(i, k,0);
	}
	return s;
}

solution construct_VNS(Data & d)
{
	return solution();
}

solution construct_Para(Data & d)
{
	return solution();
}

void vnsGraphTuning(Point seq[], int vertex_count, int ridetime, int horizent)
{
	for (int i = 1; i <= vertex_count / 2; i++)
	{
		seq[i].time_window_start = std::max(0, seq[i + vertex_count / 2].time_window_start - ridetime - seq[i].service_time);
		seq[i].time_window_end = std::min((double)horizent, seq[i + vertex_count / 2].time_window_end - dis(seq[i], seq[i + vertex_count / 2]) - seq[i].service_time);
		seq[i + vertex_count / 2].time_window_start = seq[i].time_window_start + seq[i].service_time + dis(seq[i], seq[i + vertex_count / 2]);
		seq[i + vertex_count / 2].time_window_end = std::min(seq[i].time_window_end + seq[i].service_time + ridetime, horizent);
	}

}


