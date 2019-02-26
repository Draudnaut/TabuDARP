#include "pch.h"
#include "solution.h"

solution::solution(const solution & s)
{
	length = s.length;
	cost = s.cost;
	feasibility = s.feasibility;
	for (int i = 0; i < length; i++)
	{
		tourlist[i] = s.tourlist[i];
	}
}

Tour& solution::get_Tour(int index)
{
	return tourlist[index];
}

int solution::get_length()
{
	return length;
}

double solution::get_cost()
{
	double sum = 0;
	for (int i = 0; i < length; i++)
		sum += tourlist[i].get_cost();
	return sum;
}

bool solution::get_feasibility()
{
	return feasibility;
}

void solution::update(Data &d)
{
	/*
		1. update every tour's status
		2. update cost
		3. update feasibility
	*/
	puts("update solution");
	for (int i = 0; i < length; i++)
		tourlist[i].update(d);
	feasibility = false;
	for (int i = 0; i < length; i++)
		feasibility &= tourlist[i].get_feasibility();
	cost = 0;
	for (int i = 0; i < length; i++)
		cost += tourlist[i].get_cost();
}

void solution::output(const char * direction)
{
}

