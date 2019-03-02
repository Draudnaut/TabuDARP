#pragma once
#include <cstdlib>
class Memory
{
private:
	int request_belong[1005];
	int RecordOfRequestMove[1005];
public:
	Memory();
	void update_request_memory(int Tourid, int request);
	int get_request_tour(int request);
	void updateRecordOfRequestMove(int request, int time);
	int getRecordOfRequestMove(int request);
};