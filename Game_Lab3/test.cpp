#include "Robot.h"
#include <iostream>
#include <vector>
#include "Game.h"

using namespace Game;
using namespace std;
using namespace Robots;

class Robot_Controller {
	private:
		bool check_move(movement m) {
			vector<block> neibours = main_map->get_robot_collector_neibourhood();
			switch (m) {
			case movement::left: return (neibours[0] != bomb && neibours[0] != rock);
			case movement::up: return (neibours[1] != bomb && neibours[1] != rock);
			case movement::right: return (neibours[2] != bomb && neibours[2] != rock);
			case movement::down: return (neibours[3] != bomb && neibours[3] != rock);
			default: return false;
			}
		}

		void DFS(int depth, int N, movement unmove) {
			if (depth == N || depth < N) this->move_collector(unmove);

			if (unmove != movement::left && check_move(movement::left)) DFS(depth + 1, N, movement::right);
			if (unmove != movement::up && check_move(movement::up)) DFS(depth + 1, N, movement::down);
			if (unmove != movement::right && check_move(movement::right)) DFS(depth + 1, N, movement::left);
			if (unmove != movement::down && check_move(movement::down)) DFS(depth + 1, N, movement::up);

			this->move_collector(unmove);
		}
	public:
		Main_map* main_map;
		Robot_Collector* RC;
		Robot_Sapper* RS;


		void create_RC() {
			this->RC = new Robot_Collector();
			main_map->create_robot_collector();
		}

		bool move_collector(movement m) {
			if (RC->move(m) == true) {
				main_map->move_robot_collector(m);
				main_map->update_robot_collector_existence();
				if (main_map->robot_collector_exist() == false) {
					cout << "jopda" << endl;
				}
				else {
					RC->scan(main_map->get_robot_collector_neibourhood());
				}
				return true;
			}
			else {
				cout << "nothing" << endl;
				return false;
			}
		}

		std::vector<std::vector<block>> render_map(int render_length, int render_width, int robot_screen_x, int robot_screen_y) {
			Robot_Playground* rpg = this->RC->get_map();
			std::vector<std::vector<block>> robot_map = rpg->get_renderer_map();
			std::pair<int, int> shift = rpg->get_shift();
			std::pair<int, int> rc_coords = this->RC->get_coord_on_his_own_map();
			int length = rpg->get_length();
			int width = rpg->get_width();
			std::vector<std::vector<block>> result(render_length, std::vector<block>(render_width, block::unknown));
			//for (int i = 0; i < render_length; ++i) {
			//	for (int j = 0; j < render_width; ++j) {
			//		result[i][j] = block::bomb;
			//	}
			//}e
			//cout << "size: " << length << ' ' << width << endl;
			//cout << "shift: " << shift.first << ' ' << shift.second << endl;
			//cout << "rc_coords: " << rc_coords.first << ' ' << rc_coords.second << endl;
			//return result;
			for (int i = 0; i < length; ++i) {
				for (int j = 0; j < width; ++j) {
					int x = robot_screen_x + i - rc_coords.first - shift.first;
					int y = robot_screen_y + j - rc_coords.second - shift.second;
					
					if ((x >= 0) && (x <= (render_length - 1)) && (y >= 0) && (y <= (render_width - 1))) {
						result[x][y] = robot_map[i][j];
					}
					else {
						continue;
					}
				}
			}

			for (int i = 0; i < render_length; ++i) {
				for (int j = 0; j < render_width / 2; ++j) {
					block buff = result[i][j];
					result[i][j] = result[i][render_width - j - 1];
					result[i][render_width - j - 1] = buff;
				}
			}
			return result;
		}

		void move_sapper(movement m) {
			if (RS->move(m) == true) {
				main_map->move_robot_sapper(m);
				//main_map->update_robot_collector_existence();
				//RC->scan(main_map->get_robot_collector_neibourhood());
				return;
			}
			else {
				cout << "nothing" << endl;
				return;
			}
		}

		void scan(int N){
			if (check_move(movement::left)) DFS(1, N, movement::left);
			if (check_move(movement::up)) DFS(1, N, movement::up);
			if (check_move(movement::right)) DFS(1, N, movement::right);
			if (check_move(movement::down)) DFS(1, N, movement::down);
			return;
		}

		void sapper_on() {
			this->main_map->update_robot_sapper_existence(true);

		}

		void sapper_off() {
			delete RC;
			this->main_map->update_robot_sapper_existence(false);
		}

		void auto_mode() {

		}



};