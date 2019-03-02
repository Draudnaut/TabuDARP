#pragma once
#include <cstdlib>
class Memory
{
private:
	int request_belong[1005];
public:
	Memory();
	void update_request_memory(int Tourid, int request);
	int get_request_tour(int request);
};