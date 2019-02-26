#include "pch.h"
#include "solution.h"

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

void solution::update()
{
	/*
		1. update every tour's status
		2. update cost
		3. update feasibility
	*/
	for (int i = 0; i < length; i++)
		tourlist[i].update();
	feasibility = false;
	for (int i = 0; i < length; i++)
		feasibility &= tourlist[i].get_feasibility();
	cost = 0;
	for (int i = 0; i < length; i++)
		cost += tourlist[i].get_cost();
}
