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
		fscanf(f, "%d%lf%lf%d%d%lf%lf", &point[i].id, &point[i].x, &point[i].y, &point[i].service_time, &point[i].quality_good, &point[i].time_window_start, &point[i].time_window_end);
	}
	for (int i = 0; i <= vertex_count; i++)
	{
		for (int j = 0; j <= vertex_count; j++)
		{
			matrix[i][j] = -1;
		}
	}
	for (int i = 0; i <= vertex_count; i++)
	{
		for (int j = 0; j <= vertex_count; j++)
		{
			if (i != j)
			{
				matrix[i][j] = matrix[j][i] = dis(point[i], point[j]);
			}
		}
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

double Data::getDistancePoint(int i, int j)
{
	return matrix[i][j];
}

std::vector<Point> Data::pointArray()
{
	return std::vector<Point>(point,point+vertex_count);
}

bool Data::isTightenWindow(int pointId)
{
	if (point[pointId].time_window_start!=0 or point[pointId].time_window_end!=horizent) return true;
	else return false;
}

void Data::printTimeWindow()
{
	for (int i = 0; i <= vertex_count; i++) 
		printf("point %d , time window start : %.4lf , time window end : %.4lf\n", i, point[i].time_window_start, point[i].time_window_end);
}

void Data::pruning()
{
	for (int i = 1; i <= vertex_count; i++)
	{
		if (isTightenWindow(i)==false and PickupOrDelivery(vertex_count,i)==pickup)
		{
			point[i].time_window_start = std::max(0.0, (double)point[i + vertex_count / 2].time_window_start - ridetime - 10);
			point[i].time_window_end = std::min(1440.0, point[i + vertex_count / 2].time_window_end - dis(point[i], point[i + vertex_count / 2]) - 10);
		}
		if (isTightenWindow(i)==false and PickupOrDelivery(vertex_count, i) == delivery)
		{
			point[i].time_window_start = point[i - vertex_count / 2].time_window_start + 10 + ridetime;
			point[i].time_window_end = std::min(1440.0,point[i - vertex_count / 2].time_window_end + 10 + ridetime);
		}
	}
	for (int i = 1; i <= vertex_count; i++)
	{
		for (int j = 1; j <= vertex_count; j++)
		{
			if (j == i) continue;
			if (point[i].time_window_start + dis(point[i], point[j]) > point[j].time_window_end)  
				infeasibleEdge.insert(std::make_pair(i, j));
		}
	}
}
