#include "pch.h"
#include "Data.h"

Data::Data(const char * path)
{
	FILE *f = fopen(path, "r");
	char dataset[200];
	fscanf(f, "%s", dataset);
	strcpy(this->dataset, dataset);
	fscanf(f, "%d%d%d%d%d", &vehicle_number, &vertex_count, &maximum_distance_vehicle, &capacity, &ridetime);
	for (int i = 0; i < vertex_count; i++)
	{
		fscanf(f, "%d%lf%lf%d%d%d%d", &point[i].id, &point[i].x, &point[i].y, &point[i].service_time, &point[i].quality_good, &point[i].time_window_start, &point[i].time_window_end);
	}
}
