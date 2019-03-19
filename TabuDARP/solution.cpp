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

double solution::get_cost(Parameter &p, Data &d)
{
	double sum = 0;
	for (int i = 0; i < length; i++)
	{
		if (tourlist[i].get_length()>0) sum += tourlist[i].get_cost(p, d);
	}
	return sum;
}

bool solution::get_feasibility()
{
	return feasibility;
}

void solution::update(Parameter &p,Data &d)
{
	/*
		1. update every tour's status
		2. update cost
		3. update feasibility
	*/
	for (int i = 0; i < length; i++)
		if (tourlist[i].get_length()>0) tourlist[i].update(d);
	feasibility = true;
	for (int i = 0; i < length; i++)
		feasibility &= tourlist[i].get_feasibility();
	cost = 0;
	for (int i = 0; i < length; i++)
	{
		if (tourlist[i].get_length()>0) cost += tourlist[i].get_cost(p,d);
		if (cost < 0) { int *num = NULL; *num; }
	}
		
}

void solution::output(const char * direction)
{
	for (int i = 0; i < length; i++)
	{
		//output fucntion
		tourlist[i].output();
	}
}

bool solution::operator==(solution & s)
{
	bool result = false;
	if (s.length != length) return false;
	if (fabs(cost-s.cost)>EPS) return false;
	else
	{
		
		for (int i = 0; i < length; i++) 
		{
			bool temp = false;
			for (int j = 0; j < s.length; j++)
			{
				if (tourlist[i] == s.tourlist[j]) {
					temp = true;
					break;
				}
			}
			result &= temp;
		}
	}
	return result;
}

bool solution::operator< (const solution & s) const
{
	return cost<s.cost;
}

double solution::hard_cost()
{
	double sum = 0;
	for (int i = 0; i < length; i++)
	{
		if (tourlist[i].get_length()>0) sum += tourlist[i].hard_cost();
	}
	return sum;
}

void solution::checkFeasibility(Data & d)
{
	for (int i = 0; i < length; i++)
	{
		printf("tour %d check feasibility.\n", i);
		tourlist[i].checkFeasibility(d);
	}
}

