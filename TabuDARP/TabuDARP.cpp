// TabuDARP.cpp : This file contains the 'main' function. Progr3fam execution begins and ends there.
//

#include "pch.h"
#include <iostream>
Point poi[maxn];
double depart[maxn];
double arrive[maxn];
int count_request_route[maxn];
void delete_node(int node[], int poi)
{
	int index = -1;
	for (int i = 1; i <= node[0]; i++)
	{
		if (node[i] == poi) { index = i; break; }
	}
	for (int i = index; i < node[0]; i++)
	{
		node[i] = node[i + 1];
	}
	node[0]--;
}
void insertion(int node[],int poi,int pos)
{
	for (int i = node[0]; i >= pos; i--)
	{
		node[i + 1] = node[i];
	}
	node[pos] = poi;
	node[0]++;
}
struct solution
{
	int vehicle_count;
	int tour[maxn][maxn];
	int weight[maxn][maxn];
	double cost() {
		double cost = 0;
		for (int i = 1; i <= vehicle_count; i++) {
			for (int j = 1; j <= tour[i][0]; j++) {
				if (j == 1)
				{
					cost += dis(0, tour[i][j]);
				}
				else
				{
					cost += dis(tour[i][j - 1], tour[i][j]);
				}
			}
			cost += dis(tour[i][tour[i][0]], 0);
		}
		return cost;
	}
	double isvalid(int capacity) {
		for (int i = 1; i <= vehicle_count; i++){
			for (int j = 1; j <= tour[i][0]; j++){
				if (weight[i][j] > capacity) return false;
			}
		}
		for (int i = 1; i <= vehicle_count; i++){
			for (int j = 1; j <= tour[i][0]; j++){
				if (arrive[tour[i][j]] > poi[tour[i][j]].time_window_end) return false;
			}
		}
		return true;
	}

};
std::pair<int, int> req_start_end[maxn]; //request index start and end.
int request_belong[maxn];
double alpha, beta, gamma, sigma, tao,lambda,theta;
double uniform_distribution(double a, double b){return (double)rand() / RAND_MAX * (b - a) + a;}
double punish_function(double lambda, int n, int m, int ro_ik, double cost) 
{ 
	/*
	* parameter meanings:
	  n : request number
	  m : vehicle number
	  ro_ik : the number of request i has been added to the route k
	  lambda: user-identified number
	*/
	return lambda * cost*sqrt((double)n*m)*ro_ik; 
}
bool isPickup(int point_index, int vertex_count){return point_index <= vertex_count/2;}
bool isCorespondDelivery(int a, int b, int vertex_count){return 2 * b == 2 * a + vertex_count;}
double dis(int x, int y){return sqrt((poi[x].x-poi[y].x)*(poi[x].x-poi[y].x)+ (poi[x].y - poi[y].y)*(poi[x].y - poi[y].y));}
void debug(int vehicle_number,solution s){
	for (int i = 1; i <= vehicle_number; i++){
		printf("vehicle %d : ", i);
		for (int j = 1; j <= s.tour[i][0]; j++)
			printf("%d ", s.tour[i][j]);
		puts("");
	}
}
void read_data(const char *path,int &vehicle_number,int &vertex_count,int &maximum_distance_vehicle,int &capacity,int &ridetime){
	FILE *f = fopen(path, "r");
	char dataset_name[207];
	fscanf(f, "%s", dataset_name);
	fscanf(f, "%d%d%d%d%d", &vehicle_number, &vertex_count, &maximum_distance_vehicle, &capacity, &ridetime);
	for (int i=0;i<vertex_count;i++)
	{
		fscanf(f, "%d%lf%lf%d%d%d%d", &poi[i].id, &poi[i].x, &poi[i].y, &poi[i].service_time, &poi[i].quality_good, &poi[i].time_window_start, &poi[i].time_window_end);
	}
}
Status evaluation(int tourid,solution s)
{
	Status res;
	if (s.tour[tourid][0]==0)
	{
		res.distance = 0;
		res.weight = 0;
		return res;
	}
	res.distance = dis(0,s.tour[tourid][1]);
	for (int i = 2; i <= s.tour[tourid][0]; i++)
	{
		res.distance += dis(s.tour[tourid][i - 1], s.tour[tourid][i]);
	}
	res.weight = 0;
	for (int i = 1; i <= s.tour[tourid][0]; i++)
	{
		int point = s.tour[tourid][i];
		res.weight = poi[point].quality_good;
	}
	return res;
}
solution init_solu(int vehicle_number,int vertex_number,int capacity)
{
	solution s;
	for (int i = 0; i <= vehicle_number; i++)
	{
		s.tour[i][0] = 0;
	}
	for (int i = 1; i <= vertex_number / 2; i++) 
	{
		int temp_vehicle = rand() % vehicle_number + 1;
		s.tour[temp_vehicle][++s.tour[temp_vehicle][0]] = i;
		s.tour[temp_vehicle][++s.tour[temp_vehicle][0]] = i + vertex_number / 2;
		count_request_route[i]++;
	}
	return s;
}
void update_belong(int vehicle_number,int vertex_count,solution s)
{
	for (int i = 1; i <= vehicle_number; i++){
		for (int j = 1; j <= s.tour[i][0]; j++){
			if (isPickup(s.tour[i][j], vertex_count)){
				request_belong[i] = j;
				req_start_end[i].first = j;
				for (int k = j; k <= s.tour[i][0]; k++){
					if (isCorespondDelivery(s.tour[i][j],s.tour[i][k],vertex_count)) {
						req_start_end[i].second = k;
						break;
					}
				}
			}
		}
	}
}
int main(int argc,char* argv[])
{
	const char *path = "E:/dataset/data144_13.txt";
	int vehicle_number, vertex_count, maximum_distance_vehicle, capacity, ridetime;
	read_data(path,vehicle_number, vertex_count, maximum_distance_vehicle, capacity, ridetime);
	memset(depart, 0, sizeof(depart));
	memset(arrive, 0, sizeof(arrive));
	memset(count_request_route, 0, sizeof(count_request_route));
	solution s0;
	for (int i = 0; i < vehicle_number; i++)
	{
		s0.tour[i][0] = 0;
	}
	s0 = init_solu(vehicle_number, vertex_count, capacity);
	update_belong(vehicle_number, vertex_count, s0);
	//debug(vehicle_number);
	clock_t start, end;
	start = clock();
	end = start;
	lambda = 0.015, sigma = 0.5, theta = 7.5*log10f(vehicle_number);
	alpha = 100, beta = 100, gamma = 100;
	while (end - start <= 60*CLOCKS_PER_SEC) //count one minute 
	{
		
		sigma = uniform_distribution(0, 0.5);
		lambda = uniform_distribution(0, 0.015);
		theta = uniform_distribution(0, 7.5*log10f(vehicle_number)); 
		end = clock();
	}
	return 0;
}
