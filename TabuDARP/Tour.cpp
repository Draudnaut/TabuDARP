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
		printf("index out of range in tour set\n");
		exit(-1);
	}
	nodelist[index] = node;
}

int Tour::get_length()
{
	return len;
}

void Tour::update()
{
	
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

void Tour::insert_node(int index, int node)
{
	for (int i = index; i < len; i++)
		nodelist[i + 1] = nodelist[i];
	nodelist[index] = node;
	len++;
}
