#define _CRT_SECURE_NO_WARNINGS
#include "Robot.h"
#include "enums.h"

using namespace Robots;

//----------------------------------------------------------------------------
//----------------------Robot methods-----------------------------------------
//----------------------------------------------------------------------------

Robot::Robot() {
}

//----------------------------------------------------------------------------
//----------------------Robot Collector methods-------------------------------
//----------------------------------------------------------------------------

Robot_Collector::Robot_Collector() {
	this->apples = 0;
	this->my_x = 0;
	this->my_y = 0;
	this->map = new Robot_Playground();
}

void Robot_Collector::move(movement m) {
	switch (m) {
		case right: {	++this->my_x;	break;	}
		case left: {	--this->my_x;	break; }
		case down: {	--this->my_y;	break;	}
		case up: {		++this->my_y;	break;	}
		default: {						return; }
	}

	if (this->map->robot_on_border(this->my_x, this->my_y)) {
		this->map->increase_map(m);
	}

	return;
}

void Robot_Collector::scan(std::vector<block> neighbourhood) {
	this->map->put(this->my_x - 1, this->my_y, neighbourhood[0]);
	this->map->put(this->my_x, this->my_y + 1, neighbourhood[1]);
	this->map->put(this->my_x + 1, this->my_y, neighbourhood[2]);
	this->map->put(this->my_x, this->my_y - 1, neighbourhood[3]);
	this->map->put(this->my_x, this->my_y, neighbourhood[4]);
	return;
}

void Robot_Collector::grab() {
	++this->apples;
	this->map->put(this->my_x, this->my_y ,empty);
	return;
}

int Robot_Collector::get_apples() {
	return this->apples;
}

Robot_Playground* Robot_Collector::get_map() {
	return this->map;
}

std::pair<int, int> Robot_Collector::get_coord_on_his_own_map() {
	return std::pair<int, int> { this->my_x, this->my_y };
}

//----------------------------------------------------------------------------
//----------------------Robot Sapper methods----------------------------------
//----------------------------------------------------------------------------

Robot_Sapper::Robot_Sapper(Robot_Playground& rpg, int my_x, int my_y, int collector_x, int collector_y) {
	this->my_x = my_x;
	this->my_y = my_y;
	this->collector_x = collector_x;
	this->collector_y = collector_y;
	this->map = new Robot_Playground();
}

void Robot_Sapper::move(movement m) {
	switch (m) {
		case right: {	++this->my_x;	return;	}
		case left: {	--this->my_x;	return; }
		case down: {	--this->my_y;	return;	}
		case up: {		++this->my_y;	return;	}
		default: {						return; }
	}
	return;
}

void Robot_Sapper::update_collector_coords(movement m) {
	switch (m) {
		case right: {	++this->collector_x;	return;	}
		case left: {	--this->collector_x;	return; }
		case down: {	--this->collector_y;	return;	}
		case up: {		++this->collector_y;	return;	}
		default: {								return; }
		}
	return;
}

//Robot_Sapper::~Robot_Sapper() {
//
//}

void Robot_Sapper::demine() {
	this->map->put(this->my_x, this->my_y, empty);
	return;
}

block Robot_Sapper::get_block(int shift_x, int shift_y) {
	return this->map->get_data(this->my_x + shift_x, this->my_y + shift_y);
}

std::pair<int, int> Robot_Sapper::get_coord_on_his_own_map() {
	return std::pair<int, int> { this->my_x, this->my_y };
}

Robot_Playground* Robot_Sapper::get_map() {
	return this->map;
}
