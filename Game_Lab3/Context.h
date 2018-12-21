#pragma once
#include <iostream>
#include <vector>
#include "enums.h"
#include "Main_map.h"
#include "Robot.h"

using namespace std;
using namespace Robots;

class Context {
private:
	Main_map* map;
	Robot_Collector* RC;
	Robot_Sapper* RS;
public:
	Context(const char* path);
	~Context();
};