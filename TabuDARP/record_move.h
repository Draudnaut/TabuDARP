#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
class Record_move
{
private:
	int countRequestRoute[200][20];
	int historyRequestRoute[200][20];
public:
	Record_move();
	Record_move(Record_move &rm);
	void setRecord(int request, int route,int iterator);
	int getCountRecord(int request, int route);
	bool isOkToOperate(int request, int tour, int iterator,int refer);
};