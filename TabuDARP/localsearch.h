#pragma once
#include "parameter.h"
#include "solution.h"
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
solution getNeighbors(solution s);
void TabuSearch(solution s, Parameter p);
void VariableNeighborSearch(solution s, Parameter p);
void paraNeighborSearch(solution s, Parameter p);