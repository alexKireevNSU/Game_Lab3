#include "Robot.h"
#include <iostream>
#include <vector>
#include "Game.h"

using namespace Game;
using namespace std;
using namespace Robots;

class MAIN {
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
			this->RC = new Robot_Collector();//test
			main_map->create_robot_collector();
		}

		void move_collector(movement m) {
			if (RC->move(m) == true) {
				main_map->move_robot_collector(m);
				main_map->update_robot_collector_existence();
				if (main_map->robot_collector_exist() == false) {
					cout << "jopda" << endl;
				}
				else {
					RC->scan(main_map->get_robot_collector_neibourhood());
				}
				return;
			}
			else {
				cout << "nothing" << endl;
				return;
			}
		}

		void render_map() {
			Robot_Playground* rpg = this->RC->get_map();
			std::vector<std::vector<block>> rendering_map = rpg->get_renderer_map();
			std::pair<size_t, size_t> shift = rpg->get_shift();
			std::pair<size_t, size_t> rc_coords = this->RC->get_coord_on_his_own_map();
			int length = rpg->get_length();
			int width = rpg->get_width();

		}

		void move_sapper(movement m) {
			if (RS->move(m) == true) {
				main_map->move_robot_sapper(m);
				//main_map->update_robot_collector_existence();
					RC->scan(main_map->get_robot_collector_neibourhood());
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