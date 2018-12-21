#pragma once
#include <vector>
#include "enums.h"


class Playground {
private:
	block** map;
	int length, width;
public:
	Playground();
	Playground(const char* path);
	Playground(Playground & pg);
	void increase_map(movement m);
	std::vector<std::vector<block>> get_renderer_map();
	block get_data(int, int);
	void put(int, int, block);
	int get_length();
	int get_width();
	virtual ~Playground();
};

class Robot_Playground {
private:
	Playground* map;
	int shift_x, shift_y;
public:
	Robot_Playground();
	Robot_Playground(Robot_Playground & pg);
	void increase_map(movement m);
	std::vector<std::vector<block>> get_renderer_map();
	bool robot_on_border(int robot_x, int robot_y);
	block get_data(int, int);
	void put(int, int, block);
	std::pair<int, int> get_shift();
	int get_length();
	int get_width();
	virtual ~Robot_Playground();
};
