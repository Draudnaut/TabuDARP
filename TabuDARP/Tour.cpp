#include "pch.h"
#include "Tour.h"
#include <algorithm>
Tour::Tour()
{
	len = 0; 
	feasibility = true; 
	depart[0] = 0;
	arrive[0] = 0;
	weight[0] = 0;
	distance[0] = 0;
}

Tour::Tour(const Tour & t)
{
	len = t.len;
	for (int i = 0; i < len; i++)
	{
		nodelist[i] = t.nodelist[i];
		depart[i] = t.depart[i];
		arrive[i] = t.arrive[i];
		weight[i] = t.weight[i];
		distance[i] = t.distance[i];
		feasibility = t.feasibility;
	}
}

bool Tour::get_feasibility()
{
	return feasibility;
}

double Tour::get_cost(Parameter &p,Data &d)
{
	double ans = distance[len - 1];
	double alpha = p.get_alpha();
	double beta = p.get_beta();
	double gamma = p.get_gamma();
	double tao = p.get_tao();
	ans += alpha * violation_quality(d) + beta * violation_duration(d) + gamma * violation_window(d) + tao * violation_ridetime(d);
	return ans;
}

void Tour::set_node(int index, int node)
{
	if (index >= len)
	{
		printf("index out of range in tour set.\n");
		exit(-1);
	}
	nodelist[index] = node;
}

int Tour::get_length()
{
	return len;
}

void Tour::update(Data &d)
{
	feasibility = true;
	distance[0] = dis(d.get_point(0),d.get_point(nodelist[0]));
	weight[0] = d.get_point(nodelist[0]).quality_good;
	depart[0] = 0;
	for (int i = 1; i < len; i++)
	{
		distance[i] = distance[i - 1] + dis(d.get_point(nodelist[i - 1]),d.get_point(nodelist[i]));
		weight[i] = weight[i - 1] + d.get_point(nodelist[i]).quality_good;
		arrive[i] = depart[i - 1] + dis(d.get_point(nodelist[i-1]),d.get_point(nodelist[i]));
		depart[i] = arrive[i] + 10;
		if (distance[i] > d.get_maximum_distance_vehicle()) { feasibility = false;}
	}
	distance[len] = distance[len - 1] + dis(d.get_point(0), d.get_point(nodelist[len - 1]));
	arrive[len] = depart[len - 1] + dis(d.get_point(0), d.get_point(nodelist[len - 1]));
	weight[len] = weight[len - 1];
	for (int i = 0; i < len; i++)
	{
		if (PickupOrDelivery(d.get_vertex_number(),nodelist[i]) == delivery) {
			for (int j = 0; j < i; j++) {
				if (nodelist[j] - nodelist[i] == d.get_vertex_number() / 2) {
					if (arrive[j]-depart[i]>d.get_ridetime()) {
						feasibility = false;
						break;
					}
				}
			}
		}
	}
	for (int i = 0; i < len; i++)
	{
		if (d.get_point(nodelist[i]).time_window_end<arrive[i])
		{
			feasibility = false;
			break;
		}
	}
}

void Tour::delete_node(int node)
{
	int index = -1;
	for (int i = 0; i < len; i++)
	{
		if (nodelist[i] == node)
		{
			index = i;
			break;
		}
	}
	if (index == -1) return;
	for (int i = index; i < len-1; i++)
	{
		nodelist[i] = nodelist[i+1];
	}
	len--;
}

void Tour::insert_node(int index, int node,Data &d)
{
	for (int i = len; i > index; i--)
		nodelist[i] = nodelist[i-1];
	nodelist[index] = node;
	len++;
	update(d);
}

void Tour::output()
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ", nodelist[i]);
	}
	puts("");
}

double Tour::violation_quality(Data &p)
{
	double sum = 0.0;
	for (int i = 0; i < len; i++)
	{
		sum += std::max(0,weight[i]-p.get_capacity());
	}
	return sum;
}

double Tour::violation_duration(Data & p)
{
	return std::max(0.0,distance[len]-(double)p.get_maximum_distance_vehicle());
}

double Tour::violation_window(Data & p)
{
	double sum = 0.0;
	for (int i = 0; i < len; i++)
	{
		sum += std::max(0.0,arrive[i] - (double)p.get_point(nodelist[i]).time_window_end);
	}
	return sum;
}

double Tour::violation_ridetime(Data & p)
{
	double ans = 0.0;
	for (int i = 0; i < len; i++)
	{
		if (delivery == PickupOrDelivery(p.get_vertex_number(), nodelist[i]))
		{
			for (int j = 0; j < i; j++)
			{
				if (isCorrespondPD(p.get_vertex_number(), nodelist[i], nodelist[j])) {
					double ridetime_real = arrive[i] - depart[j];
					ans += std::max(0.0, ridetime_real - (double)p.get_ridetime());
					break;
				}
			}
		}
	}
	return ans;
}

bool Tour::operator==(const Tour & t) const
{
	if (len != t.len) return false;
	bool result = true;
	for (int i = 0; i < len; i++)
	{
		if (nodelist[i] != t.nodelist[i]) result = false;
	}
	return result;
}

double Tour::hard_cost()
{
	return distance[len-1];
}

std::vector<int> Tour::get_nodelist()
{
	return std::vector<int>(nodelist,nodelist+len);
}

void Tour::checkFeasibility(Data &d)
{
	/*time window feasibility*/
	for (int i = 0; i < len; i++)
	{
		if (arrive[i] > d.get_point(nodelist[i]).time_window_end)
		{
			printf("time window violation\n");
			printf("Point %d , Time Window End : %.3lf , Arrive at %.3lf\n", d.get_point(nodelist[i]).id, d.get_point(nodelist[i]).time_window_end, arrive[i]);
		}
	}
	/*weight feasibility*/
	for (int i = 0; i < len; i++)
	{
		if (weight[i] > d.get_capacity())
			printf("weight violation\n");
	}
	/*ridetime feasibility*/
	for (int i = 0; i < len; i++)
	{
		if (PickupOrDelivery(d.get_vertex_number(),nodelist[i]) == pickup)
		{
			for (int j = i + 1; j < len; j++)
			{
				if (isCorrespondPD(d.get_vertex_number(), nodelist[i], nodelist[j]))
				{
					if (arrive[j] - depart[i] > d.get_ridetime())
					{
						printf("violation of ridetime\n");
					}
				}
			}
		}
	}
	/*duration feasibilty*/
	if (distance[len]>d.get_maximum_distance_vehicle())
	{
		printf("duration feasibility violation\n");
	}
}