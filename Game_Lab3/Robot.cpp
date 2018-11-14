#define _CRT_SECURE_NO_WARNINGS
#include "Robot.h"

using namespace Robots;

//----------------------------------------------------------------------------
//----------------------Playground methods------------------------------------
//----------------------------------------------------------------------------

Playground::Playground() {
	this->length = 0;
	this->width = 0;
	this->map = nullptr;
}
Playground::Playground(FILE* fin) {
	fscanf(fin, "%d %d", &this->length, &this->width);
	this->map = new block[this->length * this->width];
	for (size_t i = 0; i < this->length; ++i) {
		for (size_t j = 0; j < this->width; ++j) {
			char buff;
			fscanf(fin, "%c", &buff);
			block da;
			switch (buff) {
				case ' ': { da = empty; break; }
				case '#': { da = rock; break; }
				case 'a': { da = apple; break; }
				case 'b': { da = bomb; break; }
			}
			this->map[i*this->length + j] = da;
		}
	}
}
Playground::Playground(Playground & pg){
	this->length = pg.length;
	this->width = pg.width;
	this->map = new block[this->length * this->width];
	for (size_t i = 0; i < this->length; ++i) {
		for (size_t j = 0; j < this->width; ++j) {
			this->map[i*this->length + j] = pg.map[i * this->length + j];
		}
	}
}
Playground::~Playground() {
	if (this->map != nullptr) {
		delete[] this->map;
	}
	this->map = nullptr;
}
block Playground::get_data(size_t pos_x, size_t pos_y) {
	return this->map[pos_x * this->length + pos_y];
}

void Playground::put(size_t pos_x, size_t pos_y, block b) {
	this->map[pos_x * this->length + pos_y];
	return;
}

//----------------------------------------------------------------------------
//----------------------Robot methods-----------------------------------------
//----------------------------------------------------------------------------

Robot::Robot(const char* path, SDL_Rect_ rect, Sprite_State sprite_state) : Sprite(path, rect, sprite_state) {
}
Robot::~Robot() {

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

Robot_Collector::~Robot_Collector() {
	delete this->map;
}

void Robot_Collector::move(movement m) {
	switch (m) {
	case right: {
		block b = this->map->get_data(this->my_x + 1, this->my_y);
		if (b == rock) {
			return;
		}
		++this->my_x;
	}
	case left: {
		block b = this->map->get_data(this->my_x - 1, this->my_y);
		if (b == rock) {
			return;
		}
		--this->my_x;
	}
	case down: {
		block b = this->map->get_data(this->my_x, this->my_y - 1);
		if (b == rock) {
			return;
		}
		--this->my_y;
	}
	case up: {
		block b = this->map->get_data(this->my_x, this->my_y + 1);
		if (b == rock) {
			return;
		}
		++this->my_y;
	}
	default: { return; }
	}
}

void Robot_Collector::scan(size_t N) {

}

void Robot_Collector::grab() {
	if (this->map->get_data(this->my_x, this->my_y) == apple) {
		++this->apples;
	}
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

Robot_Sapper::~Robot_Sapper() {

}

void Robot_Sapper::demine() {
	if (this->map->get_data(this->my_x, this->my_y) == bomb) {
		this->map->put(this->my_x, this->my_y, empty);
	}
}


//----------------------------------------------------------------------------
//----------------------God methods-------------------------------------------
//----------------------------------------------------------------------------
