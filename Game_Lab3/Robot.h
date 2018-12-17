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
	public:
		Playground();
		Playground(FILE* fin);
		Playground(Playground & pg);
		void increase_map(movement m);
		std::vector<std::vector<block>> get_renderer_map();
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
		bool collector_exist, sapper_exist;

		void update_map(size_t x, size_t y, block b);
	public:
		Main_map();
		Main_map(FILE* fin);
		void move_robot_collector(movement m);
		void move_robot_sapper(movement m);
		//block get_robot_block();
		void update_robot_collector_existence();
		void update_robot_sapper_existence(bool flag);
		bool robot_collector_exist();
		bool robot_sapper_exist();

		std::vector<block> get_robot_collector_neibourhood();
		void create_robot_collector();
		void create_robot_sapper(size_t x, size_t y);

		void demine();
		void grab();
		~Main_map();
	};

	class Robot_Playground{
	private:
		Playground* map;
		size_t shift_x, shift_y;
	public:
		Robot_Playground();
		Robot_Playground(Robot_Playground & pg);
		void increase_map(movement m);
		std::vector<std::vector<block>> get_renderer_map();
		bool robot_on_border(size_t robot_x, size_t robot_y);
		block get_data(size_t, size_t);
		void put(size_t, size_t, block);
		std::pair<size_t, size_t> get_shift();
		size_t get_length();
		size_t get_width();
		virtual ~Robot_Playground();
	};


	class Robot{
	public:
		Robot();
		virtual ~Robot() = 0;
	};

	class Robot_Collector : public Robot {
	private:
		Robot_Playground* map;
		size_t my_x, my_y;
		size_t apples;

	public:
		Robot_Collector();
		bool move(movement m);
		//void scan(size_t N);
		void scan(std::vector<block> neighbourhood);
		bool grab();
		std::pair<size_t, size_t> get_coord_on_his_own_map();
		std::pair<size_t, size_t> coords_for_spawn_sapper();
		Robot_Playground* get_map();
		
		//virtual ~Robot_Collector();
	};

	class Robot_Sapper : public Robot {
	private:
		Robot_Playground* map;
		size_t my_x, my_y;
	public:
		Robot_Sapper(size_t my_x, size_t my_y);
		void load_playground(Robot_Playground * pg);
		bool move(movement m);
		std::pair<size_t, size_t> get_coord_on_his_own_map();
		bool demine();

		//virtual ~Robot_Sapper();
	};

}



//bool check_not_bomb(movement m){
//	switch(m){	
//		case up: return (map->get(this->robot_collector_x, this->robot_collector_y + 1) == bomb);
//		case rigth: return (map->get(this->robot_collector_x + 1, this->robot_collector_y) == bomb);
//		case down: return (map->get(this->robot_collector_x, this->robot_collector_y - 1) == bomb);
//		case left: return (map->get(this->robot_collector_x - 1, this->robot_collector_y) == bomb);
//		default: return false;
//	}
//}
//
//void DFS(int depth, int N, movement unmove){
//	if(depth == N || depth < N) move(unmove);
//	if(unmove != up && check(up)) DFS(depth + 1, N, down);
//	if(unmove != rigth && check(up)) DFS(depth + 1, N, left);
//	if(unmove != down && check(up)) DFS(depth + 1, N, up);
//	if(unmove != left && check(up)) DFS(depth + 1, N, right);
//  move(unmove);
//}
//
//void scan(int N){
//	DFS(1, N, up);
//	DFS(1, N, right);
//	DFS(1, N, down);
//	DFS(1, N, left);
//}
