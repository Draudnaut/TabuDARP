// TabuDARP.cpp : This file contains the 'main' function. Progr3fam execution begins and ends there.
//CopyRight

#include "pch.h"
#include "Data.h"
#include "solution.h"
#include "construct.h"
#include "localsearch.h"
#include <iostream>
#include <queue>
#include <vector>
#define NONE_PARALLEL
Point poi[maxn];
double depart[maxn];
double arrive[maxn];
int count_request_route[maxn];

int main(int argc,char* argv[])
{
	/*initialization components*/
	const char *path = "E:/dataset/data144_13.txt";
	Data d(path);
	Memory m;
	solution s = construct(d,m);
	Parameter p(1,1,1,1,0.5,10);
	/*Tabu Search components*/
	TabuSearch(s, p);
	return 0;
}
