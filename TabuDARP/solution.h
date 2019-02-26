#pragma once
#include "Tour.h"
class solution
{
private:
	Tour tourlist[maxn];
	int length;
	double cost;
	bool feasibility;
public:
	solution() { feasibility = false; cost = 0; }
	int get_length();
	double get_cost();
	bool get_feasibility();
	void update();
	void output(const char* direction);
};