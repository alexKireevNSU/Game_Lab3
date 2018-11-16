#pragma once
#include "Game.h"
#include <vector>
using namespace Game;
namespace Robots {
	enum block { rock, empty, apple, bomb, unknown };
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

	class Robot_Collector : public Robot {
	private:
		Playground* map;
		size_t my_x, my_y;
		size_t shift_x, shift_y;
		size_t apples;
	public:
		Robot_Collector(const char* path, SDL_Rect_ rect, Sprite_State sprite_state);
		void move(movement m);
		void scan(size_t N);
		bool grab();
		std::pair<size_t, size_t> get_coords();
		Playground* get_map();
		virtual ~Robot_Collector();
	};

	class Robot_Sapper : public Robot {
	private:
		Playground* map;
		size_t my_x, my_y;
		size_t shift_x, shift_y;

		std::vector<movement> find_way();
	public:
		Robot_Sapper(const char* path, SDL_Rect_ rect, Sprite_State sprite_state);
		void load_playground(Playground * pg);

		void demine();

		virtual ~Robot_Sapper();
	};


	class God {
	private:
		Playground* main_map;
		size_t coll_x, coll_y, sapper_x, sapper_y;
	public:
		God();
		God(Playground * pg);
		Robot_Collector* create_Robot_Collector();
		std::vector<block> get_robot_area();
		void update_robot_coords(movement m);
		void move_Robot_Collector(movement m);
	};

	class Robot : public Game::Sprite {
	public:
		Robot(const char* path, SDL_Rect_ rect, Sprite_State sprite_state);
		virtual ~Robot() = 0;
	};

	
}