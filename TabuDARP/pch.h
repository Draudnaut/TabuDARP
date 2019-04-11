// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

// TODO: add headers that you want to pre-compile here
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <ctime>
#include <iostream>
#include <set>
struct Point
{
	int id;
	double x, y;
	int service_time;
	int quality_good;
	double time_window_start;
	double time_window_end;
};
const int maxn = 1005;
#define pickup 0
#define delivery 1
#define depot -1
#define Tabu 2
#define VNS 3
#define PARA 4
#define EPS 1e-7
#endif //PCH_H
