#include "pch.h"
#include "tool.h"

double dis(Point a, Point b)
{
	return sqrt((a.x-b.x)*(a.x - b.x)+(a.y - b.y)*(a.y - b.y));
}

int PickupOrDelivery(int vertex_count,int point)
{
	/*
	    0 : pickup
		1 : delivery
	   -1 : depot
	*/
	if (point <= vertex_count / 2) return 0;
	else if (point!=0) return 1;
	else return -1;
}
