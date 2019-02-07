#include "Main_map.h"

//----------------------------------------------------------------------------
//----------------------Main_map methods--------------------------------------
//----------------------------------------------------------------------------

Main_map::Main_map() {
	this->map = new Playground();
	this->robot_collector_x = 0;
	this->robot_collector_y = 0;
	this->robot_sapper_x = 0;
	this->robot_sapper_y = 0;
}

Main_map::Main_map(const char* path) {
	this->map = new Playground(path);
	this->robot_collector_x = 0;
	this->robot_collector_y = 0;
	this->robot_sapper_x = 0;
	this->robot_sapper_y = 0;
}



bool Main_map::robot_sapper_exist() {
	return this->sapper_exist;
}

bool Main_map::robot_collector_exist() {
	return this->collector_exist;
}

void Main_map::update_robot_collector_existence() {
	if (this->map->get_data(robot_collector_x, robot_collector_y) == bomb) {
		this->collector_exist = false;
	}
	return;
}

void Main_map::update_robot_sapper_existence(bool flag) {
	this->sapper_exist = flag;
	return;
}


block Main_map::get_robot_collector_block(int shift_x, int shift_y) {
	block result = this->map->get_data(this->robot_collector_x + shift_x, this->robot_collector_y + shift_y);
	return result;
}

void Main_map::move_robot_collector(movement m) {
	switch (m) {
		case up: {		++this->robot_collector_y;	return;	}
		case down: {	--this->robot_collector_y;	return;	}
		case right: {	++this->robot_collector_x;	return;	}
		case left: {	--this->robot_collector_x;	return;	}
		default: {									return;	}
	}
	return;
}

void Main_map::move_robot_sapper(movement m) {
	switch (m) {
		case up: {		++this->robot_sapper_y;	return;	}
		case down: {	--this->robot_sapper_y;	return;	}
		case right: {	++this->robot_sapper_x;	return;	}
		case left: {	--this->robot_sapper_x; return;	}
		default: {								return;	}
	}
	return;
}

void Main_map::create_robot_collector() {
	//srand(time(NULL));
	this->collector_exist = true;
	int length = this->map->get_length();
	int width = this->map->get_width();
	while (true) {
		int x = (rand() % (width - 1)) + 1;
		int y = (rand() % (length - 1)) + 1;
		block b = this->map->get_data(x, y);
		if (b != block::bomb && b != block::rock) {
			this->robot_collector_x = x;
			this->robot_collector_y = y;
			break;
		}
	}
	return;
}

void Main_map::create_robot_sapper(int shift_x, int shift_y) {
	this->sapper_exist = true;
	this->robot_sapper_x = this->robot_collector_x + shift_x;
	this->robot_sapper_y = this->robot_collector_y + shift_y;
	return;
}

void Main_map::demine() {
	this->update_map(this->robot_sapper_x, this->robot_sapper_y, empty);
}

void Main_map::grab() {
	this->update_map(this->robot_collector_x, this->robot_collector_y, empty);
}

Main_map::~Main_map() {
	if (this->map != nullptr) {
		delete this->map;
	}
	this->map = nullptr;
}

void Main_map::update_map(int x, int y, block b) {
	this->map->put(x, y, b);
	return;
}