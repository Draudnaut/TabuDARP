#include "pch.h"
#include "record_move.h"

Record_move::Record_move()
{
	memset(countRequestRoute, 0, sizeof(countRequestRoute));
	memset(historyRequestRoute, -1, sizeof(historyRequestRoute));
}

Record_move::Record_move(Record_move & rm)
{
	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			countRequestRoute[i][j] = rm.countRequestRoute[i][j];
			historyRequestRoute[i][j] = rm.historyRequestRoute[i][j];
		}
	}
}

void Record_move::setRecord(int request, int route,int iterator)
{
	countRequestRoute[request][route]++;
	historyRequestRoute[request][route]=iterator;
}

int Record_move::getCountRecord(int request, int route)
{
	return countRequestRoute[request][route];
}

bool Record_move::isOkToOperate(int request, int tour, int iterator,int refer)
{
	if (iterator-historyRequestRoute[request][tour]<refer) return false;
	else return true;
}
