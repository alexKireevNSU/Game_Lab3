#pragma once
#include "Game.h"
#include <vector>
#include "Playgrounds.h"

using namespace Game;

namespace Robots {

	class Robot{
	public:
		Robot();
		//virtual ~Robot() = 0;
	};

	class Robot_Collector : public Robot {
	private:
		Robot_Playground* map;
		int my_x, my_y;
		int apples;

	public:
		Robot_Collector();
		void move(movement m);
		void scan(std::vector<block> neighbourhood);
		void grab();
		int get_apples();
		std::pair<int, int> get_coord_on_his_own_map();
		Robot_Playground* get_map();
		//virtual ~Robot_Collector();
	};

	class Robot_Sapper : public Robot {
	private:
		Robot_Playground* map;
		int my_x, my_y;
		int collector_x, collector_y;
	public:
		Robot_Sapper(Robot_Playground & rpg, int my_x, int my_y, int collector_x, int collector_y);
		void move(movement m);
		void update_collector_coords(movement m);
		std::pair<int, int> get_coord_on_his_own_map();
		block get_block(int shift_x, int shift_y);
		void demine();
		//virtual ~Robot_Sapper();
	};
}
