#pragma once
#include "pch.h"
#include "record_move.h"
#include "Data.h"
#include "Tour.h"
#include "solution.h"
solution construct_Tabu(Data &d,Record_move &rm);
solution construct_VNS(Data &d);
solution construct_Para(Data &d);