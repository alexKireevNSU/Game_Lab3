#define _CRT_SECURE_NO_WARNINGS
#include "Robot.h"

using namespace Robots;

//----------------------------------------------------------------------------
//----------------------Playground methods------------------------------------
//----------------------------------------------------------------------------

Playground::Playground() {
	this->length = 3;
	this->width = 3;
	this->map = new block*[this->length];
	for (size_t i = 0; i < this->length; ++i) {
		this->map[i] = new block[this->width];
	}
	for (size_t i = 0; i < this->length; ++i) {
		for (size_t j = 0; j < this->width; ++j) {
			this->map[i][j] = unknown;
		}
	}
}
Playground::Playground(FILE* fin) {
	fscanf(fin, "%d %d", &this->length, &this->width);
	this->map = new block*[this->length];

	for (size_t i = 0; i < this->length; ++i) {
		this->map[i] = new block[this->width];
	}

	for (size_t i = 0; i < this->length; ++i) {
		for (size_t j = 0; j < this->width; ++j) {
			char buff;
			fscanf(fin, "%c", &buff);
			block b;
			switch (buff) {
				case ' ': { b = empty; break; }
				case '#': { b = rock; break; }
				case 'a': { b = apple; break; }
				case 'b': { b = bomb; break; }
			}
			this->map[i][j] = b;
		}
	}
}
Playground::Playground(Playground & pg){
	this->length = pg.length;
	this->width = pg.width;
	this->map = new block*[this->length];

	for (size_t i = 0; i < this->length; ++i) {
		this->map[i] = new block[this->width];
	}

	for (size_t i = 0; i < this->length; ++i) {
		for (size_t j = 0; j < this->width; ++j) {
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

			for (size_t i = 0; i < this->length + 1; ++i) {
				this->map[i] = new block[this->width];
			}
			for (size_t i = 0; i < this->length + 1; ++i) {
				for (size_t j = 0; j < this->width; ++j) {
					this->map[i][j] = unknown;
				}
			}
			
			for (size_t i = 0; i < this->length; ++i) {
				for (size_t j = 0; j < this->width; ++j) {
					this->map[i + 1][j] = prev[i][j];
				}
			}

			for (size_t i = 0; i < this->length; ++i) {
				delete[] prev[i];
			}
			delete[] prev;

			++this->length;
			return;
		}

		case right: {
			this->map = new block*[this->length + 1];

			for (size_t i = 0; i < this->length + 1; ++i) {
				this->map[i] = new block[this->width];
			}
			for (size_t i = 0; i < this->length + 1; ++i) {
				for (size_t j = 0; j < this->width; ++j) {
					this->map[i][j] = unknown;
				}
			}
			for (size_t i = 0; i < this->length; ++i) {
				for (size_t j = 0; j < this->width; ++j) {
					this->map[i][j] = prev[i][j];
				}
			}

			for (size_t i = 0; i < this->length; ++i) {
				delete[] prev[i];
			}
			delete[] prev;

			++this->length;
			return;
		}

		case up: {
			this->map = new block*[this->length];

			for (size_t i = 0; i < this->length; ++i) {
				this->map[i] = new block[this->width + 1];
			}
			for (size_t i = 0; i < this->length; ++i) {
				for (size_t j = 0; j < this->width + 1; ++j) {
					this->map[i][j] = unknown;
				}
			}

			for (size_t i = 0; i < this->length; ++i) {
				for (size_t j = 0; j < this->width; ++j) {
					this->map[i][j + 1] = prev[i][j];
				}
			}

			for (size_t i = 0; i < this->length; ++i) {
				delete[] prev[i];
			}
			delete[] prev;

			++this->width;
			
			return;
		}

		case down: {
			this->map = new block*[this->length];

			for (size_t i = 0; i < this->length; ++i) {
				this->map[i] = new block[this->width + 1];
			}
			for (size_t i = 0; i < this->length; ++i) {
				for (size_t j = 0; j < this->width + 1; ++j) {
					this->map[i][j] = unknown;
				}
			}

			for (size_t i = 0; i < this->length; ++i) {
				for (size_t j = 0; j < this->width; ++j) {
					this->map[i][j] = prev[i][j];
				}
			}
			for (size_t i = 0; i < this->length; ++i) {
				delete[] prev[i];
			}
			delete[] prev;

			++this->width;
			return;
		}
	}

}

block Playground::get_data(size_t pos_x, size_t pos_y) {
	return this->map[pos_x][pos_y];
}

void Playground::put(size_t pos_x, size_t pos_y, block b) {
	this->map[pos_x][pos_y] = b;
	return;
}

size_t Playground::get_length() {
	return this->length;
}

size_t Playground::get_width() {
	return this->width;
}

std::vector<std::vector<block>> Playground::get_renderer_map() {
	std::vector<std::vector<block>> result = { };
	size_t width = this->width;
	size_t length = this->length;
	result.resize(length);
	for (size_t j = 0; j < length; ++j) {
		result[j].resize(width);
	}

	for (size_t i = 0; i < length; ++i) {
		for (size_t j = 0; j < width; ++j) {
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

Main_map::Main_map(FILE* fin) {
	this->map = new Playground(fin);
	this->robot_collector_x = 0;
	this->robot_collector_y = 0;
	this->robot_sapper_x = 0;
	this->robot_sapper_y = 0;
}

std::vector<block> Main_map::get_robot_collector_neibourhood() {
	std::vector<block> neibourhood = {};
	neibourhood.push_back(this->map->get_data(this->robot_collector_x - 1, robot_collector_y));
	neibourhood.push_back(this->map->get_data(this->robot_collector_x, robot_collector_y + 1));
	neibourhood.push_back(this->map->get_data(this->robot_collector_x + 1, robot_collector_y));
	neibourhood.push_back(this->map->get_data(this->robot_collector_x, robot_collector_y - 1));
	return neibourhood;
}

void Main_map::move_collector(movement m) {
	switch (m) {

	}
}

void Main_map::move_sapper(movement m) {

}

void Main_map::create_robot_collector() {
	size_t length = this->map->get_length();
	size_t width = this->map->get_width();
	while (true) {
		size_t x = (rand() % (length - 1)) + 1;
		size_t y = (rand() % (width - 1)) + 1;
		block b = this->map->get_data(x, y);
		if (b != bomb && b != rock) {
			this->robot_collector_x = x;
			this->robot_collector_y = y;
			break;
		}
	}
}

void Main_map::create_robot_sapper(size_t x, size_t y) {
	this->robot_sapper_x = x;
	this->robot_sapper_y = y;
	return;
}
Main_map::~Main_map() {
	if (this->map != nullptr) {
		delete this->map;
	}
	this->map = nullptr;
}

void Main_map::update_map(size_t x, size_t y, block b) {
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

block Robot_Playground::get_data(size_t pos_x, size_t pos_y) {
	return this->map->get_data(pos_x + this->shift_x, pos_y + this->shift_y);
}

void Robot_Playground::put(size_t pos_x, size_t pos_y, block b) {
	this->map->put(pos_x + this->shift_x, pos_y + this->shift_y, b);
	return;
}

std::pair<size_t,size_t> Robot_Playground::get_shift() {
	return std::pair<size_t, size_t> ( this->shift_x, this->shift_y );
}

size_t Robot_Playground::get_length() {
	return this->map->get_length();
}

size_t Robot_Playground::get_width() {
	return this->map->get_width();
}

std::vector<std::vector<block>> Robot_Playground::get_renderer_map() {
	return this->map->get_renderer_map();
}

//----------------------------------------------------------------------------
//----------------------Robot methods-----------------------------------------
//----------------------------------------------------------------------------

Robot::Robot(const char* path, SDL_Rect_ rect, Sprite_State sprite_state) : Sprite(path, rect, sprite_state) {
}

//----------------------------------------------------------------------------
//----------------------Robot Collector methods-------------------------------
//----------------------------------------------------------------------------

Robot_Collector::Robot_Collector(const char* path, SDL_Rect_ rect, Sprite_State sprite_state) : Robot(path, rect, sprite_state) {
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

void Robot_Collector::scan(size_t N) {
	return;
}

void Robot_Collector::scan(std::vector<block> neighbourhood) {
	this->map->put(this->my_x - 1, this->my_y, neighbourhood[0]);
	this->map->put(this->my_x, this->my_y + 1, neighbourhood[1]);
	this->map->put(this->my_x + 1, this->my_y, neighbourhood[2]);
	this->map->put(this->my_x, this->my_y - 1, neighbourhood[3]);
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

std::pair<size_t, size_t> Robot_Collector::get_coord_on_his_own_map() {
	return std::pair<size_t, size_t> { this->my_x, this->my_y };
}

std::vector<movement> Robot_Collector::find_way() {
	return std::vector<movement> { };
}

//----------------------------------------------------------------------------
//----------------------Robot Sapper methods----------------------------------
//----------------------------------------------------------------------------

Robot_Sapper::Robot_Sapper(const char* path, SDL_Rect_ rect, Sprite_State sprite_state) : Robot(path, rect, sprite_state) {
	this->my_x = 0;
	this->my_y = 0;
	this->map = new Robot_Playground();
}

void Robot_Sapper::load_playground(Robot_Playground * pg) {
	this->map = pg;
	return;
}

std::vector<movement> Robot_Sapper::find_way() {
	return std::vector<movement> { };
}

bool Robot_Sapper::demine() {
	if (this->map->get_data(this->my_x, this->my_y) == bomb) {
		this->map->put(this->my_x, this->my_y, empty);
		return true;
	}
	return false;
}

std::pair<size_t, size_t> Robot_Sapper::get_coord_on_his_own_map() {
	return std::pair<size_t, size_t> { this->my_x, this->my_y };
}

