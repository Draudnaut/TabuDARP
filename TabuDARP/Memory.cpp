#include "pch.h"
#include "Memory.h"

Memory::Memory()
{
	memset(request_belong, -1, sizeof(request_belong));
	memset(RecordOfRequestMove, -1, sizeof(RecordOfRequestMove));
}

void Memory::update_request_memory(int Tourid, int request)
{
	request_belong[request] = Tourid;
}

int Memory::get_request_tour(int request)
{
	return request_belong[request];
}

void Memory::updateRecordOfRequestMove(int request, int time)
{
	RecordOfRequestMove[request] = time;
}

int Memory::getRecordOfRequestMove(int request)
{
	return RecordOfRequestMove[request];
}
