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
		bool move(movement m);
		//void scan(int N);
		void scan(std::vector<block> neighbourhood);
		bool grab();
		std::pair<int, int> get_coord_on_his_own_map();
		//std::pair<int, int> coords_for_spawn_sapper();
		Robot_Playground* get_map();
		
		//virtual ~Robot_Collector();
	};

	class Robot_Sapper : public Robot {
	private:
		Robot_Playground* map;
		int my_x, my_y;
	public:
		Robot_Sapper(int my_x, int my_y);
		void load_playground(Robot_Playground * pg);
		bool move(movement m);
		std::pair<int, int> get_coord_on_his_own_map();
		bool demine();

		//virtual ~Robot_Sapper();
	};
}
