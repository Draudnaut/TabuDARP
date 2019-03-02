#include "pch.h"
#include "Memory.h"

Memory::Memory()
{
	memset(request_belong, -1, sizeof(request_belong));
}

void Memory::update_request_memory(int Tourid, int request)
{
	request_belong[request] = Tourid;
}

int Memory::get_request_tour(int request)
{
	return request_belong[request];
}
