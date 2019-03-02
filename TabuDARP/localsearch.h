#pragma once
#include "parameter.h"
#include "solution.h"
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
#include "Memory.h"
solution getNeighbors(solution s,Memory &m);
solution BigRemove(solution s);
void TabuSearch(solution s, Parameter p,Memory &m,Data &d);
void VariableNeighborSearch(solution s, Parameter p);
void paraNeighborSearch(solution s, Parameter p);