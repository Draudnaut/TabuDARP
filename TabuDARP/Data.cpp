#include "pch.h"
#include "Data.h"

Data::Data(const char * path)
{
	FILE *f = fopen(path, "r");
	char dataset[200];
	fscanf(f, "%s", dataset);
	strcpy(this->dataset, dataset);
	fscanf(f, "%d%d%d%d%d", &vehicle_number, &vertex_count, &duration, &capacity, &ridetime);
	for (int i = 0; i <= vertex_count; i++)
	{
		fscanf(f, "%d%lf%lf%d%d%d%d", &point[i].id, &point[i].x, &point[i].y, &point[i].service_time, &point[i].quality_good, &point[i].time_window_start, &point[i].time_window_end);
	}
}

void Data::insertInfeasibleEdge(int i, int j)
{
	//not find
	if (infeasibleEdge.find(std::make_pair(i, j))!=infeasibleEdge.end()) 
	{
		infeasibleEdge.insert(std::make_pair(i, j));
	}
}

bool Data::EdgeFeasiblity(int i, int j)
{
	if (infeasibleEdge.find(std::make_pair(i, j)) != infeasibleEdge.end()) return true;
	else return false;
}

std::vector<Point> Data::pointArray()
{
	return std::vector<Point>(point,point+vertex_count);
}
