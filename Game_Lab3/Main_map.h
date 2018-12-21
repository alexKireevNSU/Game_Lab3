#pragma once
#include "Playgrounds.h"
#include "enums.h"


class Main_map {
private:
	Playground* map;
	int robot_collector_x, robot_collector_y, robot_sapper_x, robot_sapper_y;
	bool collector_exist, sapper_exist;

	void update_map(int x, int y, block b);
public:
	Main_map();
	Main_map(const char* path);
	void move_robot_collector(movement m);
	void move_robot_sapper(movement m);
	void update_robot_collector_existence();
	void update_robot_sapper_existence(bool flag);
	bool robot_collector_exist();
	bool robot_sapper_exist();

	std::vector<block> get_robot_collector_neibourhood();
	void create_robot_collector();
	void create_robot_sapper(int x, int y);

	void demine();
	void grab();
	~Main_map();
};