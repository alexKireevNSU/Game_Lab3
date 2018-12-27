#define _CRT_SECURE_NO_WARNINGS
#include "Playgrounds.h"
#include "enums.h"
#include <iostream>

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
	this->map = new block*[this->width];

	for (int i = 0; i < this->width; ++i) {
		this->map[i] = new block[this->length];
	}
	char tmp;
	fscanf(fin, "%c", &tmp);
	for (int i = 0; i < this->length; ++i) {
		for (int j = 0; j < this->width; ++j) {
			char buff;
			fscanf(fin, "%c", &buff);
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

	fclose(fin);
}

Playground::Playground(Playground & pg) {
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
//----------------------Robot_Playground methods------------------------------
//----------------------------------------------------------------------------

Robot_Playground::Robot_Playground() {
	this->map = new Playground();
	this->shift_x = 1;
	this->shift_y = 1;
}

Robot_Playground::Robot_Playground(Robot_Playground & pg) {
	this->map = new Playground(*pg.map);
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

std::pair<int, int> Robot_Playground::get_shift() {
	return std::pair<int, int>(this->shift_x, this->shift_y);
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
	if (((robot_x + shift_x) == (this->get_length() - 1)) || ((robot_x + shift_x) == 0)) {
		return true;
	}
	if (((robot_y + shift_y) == (this->get_width() - 1)) || ((robot_y + shift_y) == 0)) {
		return true;
	}
	return false;
}

void Robot_Playground::increase_map(movement m) {
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