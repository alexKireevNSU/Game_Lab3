#pragma once
#include <iostream>
#include <vector>
#include "enums.h"
#include "Main_map.h"
#include "Robot.h"

using namespace std;
using namespace Robots;

class Context {
public:
	Main_map* map;
	Robot_Collector* RC;
	Robot_Sapper* RS;
	Context(const char* path);
	Context();
	~Context();
};