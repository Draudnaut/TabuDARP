#pragma once
#include "Tour.h"
class solution
{
private:
	Tour tourlist[200];
	int length;
	double cost;
	bool feasibility;
public:
	solution() { feasibility = false; cost = 0; length = 0; }
	solution(const solution & s);
	Tour &get_Tour(int index);
	int get_length();
	double get_cost();
	bool get_feasibility();
	void update();
	void output(const char* direction);
	void set_length(int len) { length = len; }
	void operator = (const solution &s);
};