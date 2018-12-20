#define _CRT_SECURE_NO_WARNINGS
#include "Robot.h"
#include <time.h>

using namespace Robots;

//----------------------------------------------------------------------------
//----------------------Playground methods------------------------------------
//----------------------------------------------------------------------------

Playground::Playground() {
	this->length = 3;
	this->width = 3;
	this->map = new block*[this->length];
	for (int i = 0; i < this->length; ++i) {
		this->map[i] = new block[this->width];
	}
	for (int i = 0; i < this->length; ++i) {
		for (int j = 0; j < this->width; ++j) {
			this->map[i][j] = unknown;
		}
	}
}
Playground::Playground(const char* path) {
	FILE* fin = fopen(path, "r");
	fscanf(fin, "%d %d", &this->length, &this->width);
	std::cout << this->length << ' ' << this->width << std::endl;
	this->map = new block*[this->length];

	for (int i = 0; i < this->length; ++i) {
		this->map[i] = new block[this->width];
	}
	char tmp;
	fscanf(fin, "%c", &tmp);
	//fscanf(fin, "%c", &tmp);
	for (int i = 0; i < this->length; ++i) {
		for (int j = 0; j < this->width; ++j) {
			char buff;
			fscanf(fin, "%c", &buff);
			//std::cout << buff << std::endl;
			block b = empty;
			switch (buff) {
			case ' ': { b = empty; break; }
			case '#': { b = rock; break; }
			case 'a': { b = apple; break; }
			case 'b': { b = bomb; break; }
			}
			this->map[j][this->length - i - 1] = b;
		}
		fscanf(fin, "%c", &tmp);
	}

	//for (int i = 0; i < this->length; ++i) {
	//	for (int j = 0; j < this->width / 2; ++j) {
	//		blo
	//	}
	//}
	fclose(fin);
}

Playground::Playground(Playground & pg){
	this->length = pg.length;
	this->width = pg.width;
	this->map = new block*[this->length];

	for (int i = 0; i < this->length; ++i) {
		this->map[i] = new block[this->width];
	}

	for (int i = 0; i < this->length; ++i) {
		for (int j = 0; j < this->width; ++j) {
			this->map[i][j] = pg.map[i][j];
		}
	}
}
Playground::~Playground() {
	if (this->map != nullptr) {
		delete[] this->map;
	}
	this->map = nullptr;
}

void Playground::increase_map(movement m) {
	block** prev = this->map;

	switch (m) {
		case left: {
			this->map = new block*[this->length + 1];

			for (int i = 0; i < this->length + 1; ++i) {
				this->map[i] = new block[this->width];
				for (int j = 0; j < this->width; ++j) {
					this->map[i][j] = unknown;
				}
			}
			
			for (int i = 0; i < this->length; ++i) {
				for (int j = 0; j < this->width; ++j) {
					this->map[i + 1][j] = prev[i][j];
				}
			}


			for (int i = 0; i < this->length; ++i) {
				delete[] prev[i];
			}
			delete[] prev;

			++this->length;
			break;
		}

		case right: {
			this->map = new block*[this->length + 1];

			for (int i = 0; i < this->length + 1; ++i) {
				this->map[i] = new block[this->width];
				for (int j = 0; j < this->width; ++j) {
					this->map[i][j] = unknown;
				}
			}

			for (int i = 0; i < this->length; ++i) {
				for (int j = 0; j < this->width; ++j) {
					this->map[i][j] = prev[i][j];
				}
			}


			for (int i = 0; i < this->length; ++i) {
				delete[] prev[i];
			}
			delete[] prev;

			++this->length;
			break;
		}
	
		case down: {
			this->map = new block*[this->length];

			for (int i = 0; i < this->length; ++i) {
				this->map[i] = new block[this->width + 1];
				for (int j = 0; j < this->width + 1; ++j) {
					this->map[i][j] = unknown;
				}
			}

			for (int i = 0; i < this->length; ++i) {
				for (int j = 0; j < this->width; ++j) {
					this->map[i][j + 1] = prev[i][j];
				}
			}


			for (int i = 0; i < this->length; ++i) {
				delete[] prev[i];
			}
			delete[] prev;

			++this->width;
			break;
		}

		case up: {
			this->map = new block*[this->length];

			for (int i = 0; i < this->length; ++i) {
				this->map[i] = new block[this->width + 1];
				for (int j = 0; j < this->width + 1; ++j) {
					this->map[i][j] = unknown;
				}
			}

			for (int i = 0; i < this->length; ++i) {
				for (int j = 0; j < this->width; ++j) {
					this->map[i][j] = prev[i][j];
				}
			}


			for (int i = 0; i < this->length; ++i) {
				delete[] prev[i];
			}
			delete[] prev;

			++this->width;
			break;
		}
	}


	return;
}

block Playground::get_data(int pos_x, int pos_y) {
	return this->map[pos_x][pos_y];
}

void Playground::put(int pos_x, int pos_y, block b) {
	this->map[pos_x][pos_y] = b;
	return;
}

int Playground::get_length() {
	return this->length;
}

int Playground::get_width() {
	return this->width;
}

std::vector<std::vector<block>> Playground::get_renderer_map() {
	std::vector<std::vector<block>> result = { };
	int width = this->width;
	int length = this->length;
	result.resize(length);
	for (int j = 0; j < length; ++j) {
		result[j].resize(width);
	}

	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < width; ++j) {
			result[i][j] = this->map[i][j];
		}
	}
	return result;
}


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


std::vector<block> Main_map::get_robot_collector_neibourhood() {
	std::vector<block> neibourhood = {};
	neibourhood.push_back(this->map->get_data(this->robot_collector_x - 1, robot_collector_y));
	neibourhood.push_back(this->map->get_data(this->robot_collector_x, robot_collector_y + 1));
	neibourhood.push_back(this->map->get_data(this->robot_collector_x + 1, robot_collector_y));
	neibourhood.push_back(this->map->get_data(this->robot_collector_x, robot_collector_y - 1));
	neibourhood.push_back(this->map->get_data(this->robot_collector_x, robot_collector_y));
	return neibourhood;
}

void Main_map::move_robot_collector(movement m) {
	switch (m) {
		case up: {
			++this->robot_collector_y;
			return;
		}
		case down: {
			--this->robot_collector_y;
			return;
		}
		case right: {
			++this->robot_collector_x;
			return;
		}
		case left: {
			--this->robot_collector_x;
			return;
		}
		default: {
			return;
		}
	}
	return;
}

void Main_map::move_robot_sapper(movement m) {
	switch (m) {
		case up: {
			++this->robot_sapper_y;
			return;
		}
		case down: {
			--this->robot_sapper_y;
			return;
		}
		case right: {
			++this->robot_sapper_x;
			return;
		}
		case left: {
			--this->robot_sapper_x;
			return;
		}
		default: {
			return;
		}
	}
	return;
}

void Main_map::create_robot_collector() {
	srand(time(NULL));
	this->collector_exist = true;
	int length = this->map->get_length();
	int width = this->map->get_width();
	while (true) {
		int x = (rand() % (length));
		int y = (rand() % (width));
		block b = this->map->get_data(x, y);
		if (b != bomb && b != rock) {
			this->robot_collector_x = x;
			this->robot_collector_y = y;
			break;
		}
	}
}

void Main_map::create_robot_sapper(int x, int y) {
	this->sapper_exist = true;
	this->robot_sapper_x = x;
	this->robot_sapper_y = y;
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

//----------------------------------------------------------------------------
//----------------------Robot_Playground methods------------------------------
//----------------------------------------------------------------------------

Robot_Playground::Robot_Playground() {
	this->map = new Playground();
	this->shift_x = 1;
	this->shift_y = 1;
}

Robot_Playground::Robot_Playground(Robot_Playground & pg) {
	this->map = pg.map;
	this->shift_x = pg.shift_x;
	this->shift_y = pg.shift_y;
}
Robot_Playground::~Robot_Playground() {
	if (this->map != nullptr) {
		delete[] this->map;
	}
	this->map = nullptr;
}

block Robot_Playground::get_data(int pos_x, int pos_y) {
	return this->map->get_data(pos_x + this->shift_x, pos_y + this->shift_y);
}

void Robot_Playground::put(int pos_x, int pos_y, block b) {
	this->map->put(pos_x + this->shift_x, pos_y + this->shift_y, b);
	return;
}

std::pair<int,int> Robot_Playground::get_shift() {
	return std::pair<int, int> ( this->shift_x, this->shift_y );
}

int Robot_Playground::get_length() {
	return this->map->get_length();
}

int Robot_Playground::get_width() {
	return this->map->get_width();
}

std::vector<std::vector<block>> Robot_Playground::get_renderer_map() {
	return this->map->get_renderer_map();
}

bool Robot_Playground::robot_on_border(int robot_x, int robot_y) {
	//std::cout << "robot_x on map:" << robot_x + shift_x << std::endl;
	//std::cout << "robot_y on map:" << robot_y + shift_y << std::endl;
	//std::cout << "map_size:" << this->get_length() << ' ' << this->get_width() << std::endl;

	if (((robot_x + shift_x) == (this->get_length() - 1)) || ((robot_x + shift_x) == 0)) {
		return true;
	}
	if (((robot_y + shift_y) == (this->get_width() - 1)) || ((robot_y + shift_y) == 0)) {
		return true;
	}
	//std::cout << "not_on_border" << std::endl;
	return false;
}

void Robot_Playground::increase_map(movement m) {
	//std::cout << "increasing" << std::endl;
	switch (m) {
		case movement::left: {
			++this->shift_x;
			break;
		}
		case movement::down: {
			++this->shift_y;
			break;
		}
		default: {break; }
	}

	this->map->increase_map(m);
	return;
}

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

//Robot_Collector::~Robot_Collector() {
//	delete this->map;
//}

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
			//std::cout << "true" << std::endl;
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

//void Robot_Collector::scan(int N) {
//	return;
//}
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

std::pair<int, int> Robot_Collector::coords_for_spawn_sapper() {
	int width = this->map->get_width();
	int length = this->map->get_length();
	int collector_x = this->my_x;
	int collector_y = this->my_y;
	std::pair<int, int> shift = this->map->get_shift();
	int result_x, result_y;
	while (true) {
		result_x = rand() % width - shift.first;
		result_y = rand() % length - shift.second;
		if (result_x != collector_x && result_y != collector_y && this->map->get_data(result_x, result_y) != rock) {
			break;
		}
	}
	return std::pair<int, int>(result_x, result_y);
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

