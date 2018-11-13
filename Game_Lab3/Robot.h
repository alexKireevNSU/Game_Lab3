#pragma once
#include "Game.h"
#include <vector>
using namespace Game;
namespace Robots {
	enum block { rock, empty, apple, bomb };
	enum movement { right, left, up, down };

	class Playground {
	private:
		block* map;
		size_t length, width;

	public:
		Playground();
		Playground(FILE* fin);
		Playground(Playground & pg);
		block get_data(size_t, size_t) { return empty; }

		virtual ~Playground();
	};

	class God {
	private:
		Playground main_map;
		size_t coll_x, coll_y;
	public:
		God();
		God(Playground & pg);
		void Create_Robot_Collector(size_t x, size_t y);
		std::vector<block> get_robot_area();
		void Move_Robot_Collector(movement m);
	};

	class Robot : public Game::Sprite {
	public:
		Robot();
		virtual ~Robot() = 0;
	};



	class Robot_Collector : public Robot {
	private:
		Playground map;
		size_t my_x, my_y;
		class Scan {
			Scan();
			void doing(size_t N);
		};
		class Move {
			Move();
			void doing(movement m);
		};
		class Grab {
			Grab();
			void doing();
		};
	public:
		Robot_Collector(size_t my_x, size_t my_y);
		void move(movement m);
		void scan(size_t N);
		void Grab();
		virtual ~Robot_Collector();
	};

	/*class Robot_Sapper : public Robot {
	private:
		Playground map;
		size_t my_x, my_y;

		class Demine {
			Demine();
			~Demine();
			void doing();
		};

	public:
		Robot_Sapper();
		Robot_Sapper(Robot_Collector & rc);
		Demine demine;

		virtual ~Robot_Sapper();
	};*/

	
}