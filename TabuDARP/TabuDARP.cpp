// TabuDARP.cpp : This file contains the 'main' function. Progr3fam execution begins and ends there.
//

#include "pch.h"
#include "Data.h"
#include "solution.h"
#include "construct.h"
#include <iostream>
Point poi[maxn];
double depart[maxn];
double arrive[maxn];
int count_request_route[maxn];

int main(int argc,char* argv[])
{
	const char *path = "E:/dataset/data144_13.txt";
	Data d(path);
	solution s = construct(d);
	Parameter p(1,1,1,1,0.5,10);
	printf("%d\n", s.get_length());
	s.update(p, d);
	std::cout << s.get_cost(p,d) << std::endl;
	std::cout << s.get_feasibility() << std::endl;
	return 0;
}
