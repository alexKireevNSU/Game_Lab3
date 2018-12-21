#include <iostream>
#include <vector>
#include "enums.h"
#include "Main_map.h"
#include "Robot.h"
#include "Context.h"

using namespace std;
using namespace Robots;

Context::Context(const char* path) {
	this->map = new Main_map(path);
	this->RC = new Robot_Collector();
	this->RS = nullptr;
}



Context::~Context() {
	delete this->map;
	delete this->RC;
	delete this->RS;
}