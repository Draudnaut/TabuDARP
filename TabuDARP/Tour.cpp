#include "pch.h"
#include "Tour.h"

Tour::Tour()
{
	len = 0; 
	feasibility = false; 
	time_stamp[0] = 0;
	weight[0] = 0;
	distance[0] = 0;
}

Tour::Tour(const Tour & t)
{
	len = t.len;
	for (int i = 0; i < len; i++)
	{
		nodelist[i] = t.nodelist[i];
		time_stamp[i] = t.time_stamp[i];
		weight[i] = t.weight[i];
		distance[i] = t.distance[i];
		feasibility = t.feasibility;
	}
}

bool Tour::get_feasibility()
{
	return feasibility;
}

double Tour::get_cost()
{
	return distance[len-1];
}

void Tour::set_node(int index, int node)
{
	if (index >= len)
	{
		printf("index out of range in tour set.\n");
		exit(-1);
	}
	nodelist[index] = node;
}

int Tour::get_length()
{
	return len;
}

void Tour::update(Data &d)
{
	distance[0] = dis(d.get_point(0),d.get_point(nodelist[0]));
	weight[0] = d.get_point(nodelist[0]).quality_good;
	time_stamp[0] = distance[0];
	for (int i = 1; i < len; i++)
	{
		distance[i] = distance[i - 1] + dis(d.get_point(nodelist[i - 1]),d.get_point(nodelist[i]));
		weight[i] = weight[i - 1] + d.get_point(nodelist[i - 1]).quality_good;
		time_stamp[i] = distance[i];
		if (distance[i] > d.get_capacity()) feasibility = false;
	}
	for (int i = 0; i < len; i++)
	{
		if (PickupOrDelivery(d.get_vertex_number(),nodelist[i]) == delivery) {
			for (int j = 0; j < i; j++) {
				if (nodelist[j] - nodelist[i] == d.get_vertex_number() / 2) {
					if (true) {
						feasibility = false;
						break;
					}
				}
			}
		}
	}

}

void Tour::delete_node(int index)
{
	if (index < 0 || index >= len)
	{
		printf("delete node error,out of range\n");
		exit(-1);
	}
	for (int i = index + 1; i < len; i++)
	{
		nodelist[i - 1] = nodelist[i];
	}
	len = len - 1;
}

void Tour::insert_node(int index, int node,Data &d)
{
	for (int i = index; i < len; i++)
		nodelist[i + 1] = nodelist[i];
	nodelist[index] = node;
	len++;
	update(d);
}
