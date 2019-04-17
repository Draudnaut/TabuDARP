// TabuDARP.cpp : This file contains the 'main' function. Progr3fam execution begins and ends there.
//CopyRight

#include "pch.h"
#include "Data.h"
#include "solution.h"
#include "construct.h"
#include "localsearch.h"
#include "record_move.h"
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
	/*initialization parameter*/
	const char *path = "E:/dataset/R6b.txt";
	Data d(path);
	Parameter p(1,1,1,1,0.5,10,7.5*log(d.get_vertex_number()/2),15);
	solution s;
	Record_move rm;
	int search_function = VNS;
	/*initialization*/
	if (search_function == Tabu) 
	{		
		s = construct_Tabu(d,rm); 
		
	}
	else if (search_function == VNS) 
	{ 
		d.pruning();
		d.printTimeWindow();
		s = construct_VNS(d); 
	}
	else 
	{ 
		s = construct_Para(d); 
	}
	/*Tabu Search components*/
	if(search_function==Tabu) TabuSearch(s, p, d,rm,1);
	else if (search_function == VNS) VariableNeighborSearch(s, p, d);
	else paraNeighborSearch(s, p);
	return 0;
}
