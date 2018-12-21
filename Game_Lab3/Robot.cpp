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

bool Robot_Collector::move(movement m) {
	switch (m) {
		case right: {
			block b = this->map->get_data(this->my_x + 1, this->my_y);
			if (b == rock) {
				return false;
			}
			++this->my_x;
			if (this->map->robot_on_border(this->my_x, this->my_y)) {
				this->map->increase_map(m);
			}
			return true;
		}
		case left: {
			block b = this->map->get_data(this->my_x - 1, this->my_y);
			if (b == rock) {
				return false;
			}
			--this->my_x;
			if (this->map->robot_on_border(this->my_x, this->my_y)) {
				this->map->increase_map(m);
			}
			return true;
		}
		case down: {
			block b = this->map->get_data(this->my_x, this->my_y - 1);
			if (b == rock) {
				return false;
			}
			--this->my_y;
			if (this->map->robot_on_border(this->my_x, this->my_y)) {
				this->map->increase_map(m);
			}
			return true;
		}
		case up: {
			block b = this->map->get_data(this->my_x, this->my_y + 1);
			if (b == rock) {
				return false;
			}
			++this->my_y;
			if (this->map->robot_on_border(this->my_x, this->my_y)) {
				this->map->increase_map(m);
			}
			return true;
		}
		default: { return false; }
	}
}

void Robot_Collector::scan(std::vector<block> neighbourhood) {
	this->map->put(this->my_x - 1, this->my_y, neighbourhood[0]);
	this->map->put(this->my_x, this->my_y + 1, neighbourhood[1]);
	this->map->put(this->my_x + 1, this->my_y, neighbourhood[2]);
	this->map->put(this->my_x, this->my_y - 1, neighbourhood[3]);
	this->map->put(this->my_x, this->my_y, neighbourhood[4]);
	return;
}

bool Robot_Collector::grab() {
	if (this->map->get_data(this->my_x, this->my_y) == apple) {
		++this->apples;
		this->map->put(this->my_x, this->my_y ,empty);
		return true;
	}
	return false;
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

Robot_Sapper::Robot_Sapper(int my_x, int my_y) {
	this->my_x = my_x;
	this->my_y = my_y;
	this->map = new Robot_Playground();
}

void Robot_Sapper::load_playground(Robot_Playground * pg) {
	this->map = pg;
	return;
}

bool Robot_Sapper::move(movement m) {
	switch (m) {
	case right: {
		block b = this->map->get_data(this->my_x + 1, this->my_y);
		if (b == rock) {
			return false;
		}
		++this->my_x;
	}
	case left: {
		block b = this->map->get_data(this->my_x - 1, this->my_y);
		if (b == rock) {
			return false;
		}
		--this->my_x;
	}
	case down: {
		block b = this->map->get_data(this->my_x, this->my_y - 1);
		if (b == rock) {
			return false;
		}
		--this->my_y;
	}
	case up: {
		block b = this->map->get_data(this->my_x, this->my_y + 1);
		if (b == rock) {
			return false;
		}
		++this->my_y;
	}
	default: { return false; }
	}
	return true;
}

//Robot_Sapper::~Robot_Sapper() {
//
//}

bool Robot_Sapper::demine() {
	if (this->map->get_data(this->my_x, this->my_y) == bomb) {
		this->map->put(this->my_x, this->my_y, empty);
		return true;
	}
	return false;
}

std::pair<int, int> Robot_Sapper::get_coord_on_his_own_map() {
	return std::pair<int, int> { this->my_x, this->my_y };
}