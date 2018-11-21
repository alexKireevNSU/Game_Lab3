#define _CRT_SECURE_NO_WARNINGS
#include "Robot.h"

using namespace Robots;

//----------------------------------------------------------------------------
//----------------------Playground methods------------------------------------
//----------------------------------------------------------------------------

Playground::Playground() {
	this->length = 3;
	this->width = 3;
	this->shift_x = 1;
	this->shift_y = 1;
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
	this->shift_x = 0;
	this->shift_y = 0;
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
			++this->shift_x;
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
			++this->shift_y;
			
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
	return this->map[pos_x + this->shift_x][pos_y + this->shift_y];
}

void Playground::put(size_t pos_x, size_t pos_y, block b) {
	this->map[pos_x + this->shift_x][pos_y + this->shift_y] = b;
	return;
}

size_t Playground::get_length() {
	return this->length;
}

size_t Playground::get_width() {
	return this->width;
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
	this->map = new Playground();
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
	
}

void Robot_Collector::scan(std::vector<block>) {


}

bool Robot_Collector::grab() {
	if (this->map->get_data(this->my_x, this->my_y) == apple) {
		++this->apples;
		this->map->put(this->my_x, this->my_y ,empty);
		return true;
	}
	return false;
}

Playground* Robot_Collector::get_map() {
	return this->map;
}

//----------------------------------------------------------------------------
//----------------------Robot Sapper methods----------------------------------
//----------------------------------------------------------------------------

Robot_Sapper::Robot_Sapper(const char* path, SDL_Rect_ rect, Sprite_State sprite_state) : Robot(path, rect, sprite_state) {
	this->my_x = 0;
	this->my_y = 0;
	this->map = new Playground();
}

void Robot_Sapper::load_playground(Playground * pg) {
	this->map = pg;
	return;
}

//Robot_Sapper::~Robot_Sapper() {
//
//}

//std::vector<movement> Robot_Sapper::find_way() {
//	return std::vector<movement>{};
//}

void Robot_Sapper::demine() {
	if (this->map->get_data(this->my_x, this->my_y) == bomb) {
		this->map->put(this->my_x, this->my_y, empty);
	}
}


//----------------------------------------------------------------------------
//----------------------God methods-------------------------------------------
//----------------------------------------------------------------------------

God::God() {
	
}

God::God(Playground * pg) {

}

Robot_Collector* God::create_Robot_Collector() {
	size_t x, y;
	while (true) {
		x = rand() % this->main_map->get_length();
		y = rand() % this->main_map->get_width();
		block b = this->main_map->get_data(x, y);
		if (b != rock && b != bomb) {
			break;
		}
	}
	this->coll_x = x;
	this->coll_y = y;
	return new Robot_Collector(nullptr, SDL_Rect_(0, 0, 0, 0), visible);
}

void God::move_Robot_Collector(movement m) {

}

void God::update_robot_coords(movement m) {

}

std::vector<block> God::get_robot_area() {
	std::vector<block> result;
	result.push_back(this->main_map->get_data(this->coll_x - 1, this->coll_y));
	result.push_back(this->main_map->get_data(this->coll_x, this->coll_y + 1));
	result.push_back(this->main_map->get_data(this->coll_x + 1, this->coll_y));
	result.push_back(this->main_map->get_data(this->coll_x, this->coll_y - 1));
	return result;
}