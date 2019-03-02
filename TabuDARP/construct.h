#pragma once
#include "pch.h"
#include "Memory.h"
#include "Data.h"
#include "Tour.h"
#include "solution.h"
solution construct_Tabu(Data &d,Memory &m);
solution construct_VNS(Data &d, Memory &m);
solution construct_Para(Data &d, Memory &m);