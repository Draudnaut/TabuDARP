#pragma once
#include "pch.h"
#include "record_move.h"
#include "Data.h"
#include "Tour.h"
#include "solution.h"
#include "tool.h"
#include <algorithm>
solution construct_Tabu(Data &d,Record_move &rm);
solution construct_VNS(Data &d);
solution construct_Para(Data &d);
double vnsCriterion1(Tour &t, Point a,Data &d);
double vnsCriterion2(Tour &t, Point a,Data &d);
double vnsCriterion3(Tour &t, Point a,Data &d);
double vnsCriterion4(Tour &t, Point a,Data &d);