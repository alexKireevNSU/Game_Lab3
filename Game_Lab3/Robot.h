#pragma once
#include "Game.h"
#include <vector>
using namespace Game;
namespace Robots {
	enum movement { right, left, up, down };

	class Playground {
	private:
		block** map;
		size_t length, width;
		size_t shift_x, shift_y;
		void increase_map(movement m);

	public:
		Playground();
		Playground(FILE* fin);
		Playground(Playground & pg);

		block get_data(size_t, size_t);
		void put(size_t, size_t, block);
		size_t get_length();
		size_t get_width();
		virtual ~Playground();
	};

	class Main_map {
	private:
		Playground* map;
		size_t robot_collector_x, robot_collector_y, robot_sapper_x, robot_sapper_y;
	public:
		Main_map();
		Main_map(FILE* fin);
		void move_collector(movement m);
		void move_sapper(movement m);
		std::vector<block> get_robot_collector_neibourhood();
		void create_robot_collector();
		void create_robot_sapper(size_t x, size_t y);
		void update_map(size_t x, size_t y, block b);
		~Main_map();
	};


	class Robot : public Game::Sprite {
	public:
		Robot(const char* path, SDL_Rect_ rect, Sprite_State sprite_state);
		//virtual ~Robot() = 0;
	};

	class Robot_Collector : public Robot {
	private:
		Playground* map;
		size_t my_x, my_y;
		size_t apples;
		std::vector<movement> find_way();

	public:
		Robot_Collector(const char* path, SDL_Rect_ rect, Sprite_State sprite_state);
		bool move(movement m);
		void scan(size_t N);
		void scan(std::vector<block> neighbourhood);
		bool grab();
		std::pair<size_t, size_t> get_coord_on_his_own_map();
		
		Playground* get_map();
		//virtual ~Robot_Collector();
	};

	class Robot_Sapper : public Robot {
	private:
		Playground* map;
		size_t my_x, my_y;

		std::vector<movement> find_way();
	public:
		Robot_Sapper(const char* path, SDL_Rect_ rect, Sprite_State sprite_state);
		void load_playground(Playground * pg);

		std::pair<size_t, size_t> get_coord_on_his_own_map();
		bool demine();

		//virtual ~Robot_Sapper();
	};
}