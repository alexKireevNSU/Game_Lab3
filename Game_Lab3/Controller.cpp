#include "enums.h"
#include "Robot.h"
#include "Main_map.h"
#include "Context.h"
#include "Controller.h"
#include <vector>
#include <queue>

//----------------------------------------------------------------------------
//----------------------Manual_Controller methods-----------------------------
//----------------------------------------------------------------------------

bool thereis_place_for_spawn_sapper(std::vector<std::vector<block>> pg, int length, int width, int rc_x, int rc_y) {
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < width; ++j) {
			if ((pg[i][j] != block::unknown) && (pg[i][j] != block::rock) && (i != rc_x || j != rc_y)) {
				return true;
			}
		}
	}
	return false;
}

void Manual_Controller::sapper_on(Context * context) {
	if (context->RS != nullptr) {
		std::cout << "You have one!" << std::endl;
		return;
	}
	Robot_Playground* rpg = context->RC->get_map();
	std::pair<int, int> robot_coords = context->RC->get_coord_on_his_own_map();
	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	std::pair<int, int> shift = rpg->get_shift();
	int length = rpg->get_length();
	int width = rpg->get_width();

	int rc_x = robot_coords.first + shift.first;
	int rc_y = robot_coords.second + shift.second;
	
	if (!thereis_place_for_spawn_sapper(pg, length, width, rc_x, rc_y)) {
		std::cout << "IMMPOSIBLE TO SPAWN" << std::endl;
		return;
	}

	int sapper_x, sapper_y;
	while (true) {
		sapper_x = rand() % length;
		sapper_y = rand() % width;
		if ((pg[sapper_x][sapper_y] != block::unknown) && (pg[sapper_x][sapper_y] != block::rock) && (sapper_x != rc_x || sapper_y != rc_y)) {
			break;
		}
	}
	sapper_x -= shift.first;
	sapper_y -= shift.second;
	context->RS = new Robot_Sapper(context->RC->get_map(), sapper_x, sapper_y, robot_coords.first, robot_coords.second);
	context->map->create_robot_sapper(sapper_x - robot_coords.first, sapper_y - robot_coords.second);
	return;
}
void Manual_Controller::sapper_off(Context * context) {
	delete context->RS;
	context->RS = nullptr;
	context->map->update_robot_sapper_existence(false);
	std::cout << "death" << std::endl;
	return;
}

bool Manual_Controller::move_collector(Context* context, movement m) {
	std::pair<int, int> robot_collector_coords = context->RC->get_coord_on_his_own_map();
	std::pair<int, int> robot_collector_shift = context->RC->get_map()->get_shift();
	bool sapper_exist = context->map->robot_sapper_exist();
	int robot_sapper_x = 0, robot_sapper_y = 0;
	if (sapper_exist == true) {
		std::pair<int, int> robot_sapper_coords = context->RS->get_coord_on_his_own_map();
		std::pair<int, int> robot_sapper_shift = context->RS->get_map()->get_shift();
		robot_sapper_x = robot_sapper_coords.first + robot_sapper_shift.first;
		robot_sapper_y = robot_sapper_coords.second + robot_sapper_shift.second;
	}
	

	int robot_collector_x = robot_collector_coords.first + robot_collector_shift.first;
	int robot_collector_y = robot_collector_coords.second + robot_collector_shift.second;
	
	switch (m) {
		case movement::up: {
			if (context->map->get_robot_collector_block(0, 1) == block::rock) {
				return false;
			}
			if (sapper_exist == true) {
				if ((robot_collector_x == robot_sapper_x) && (robot_collector_y + 1 == robot_sapper_y)) {
					return false;
				}
			}
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		case movement::right: {
			if (context->map->get_robot_collector_block(1, 0) == block::rock) {
				return false;
			}
			if (sapper_exist == true) {
				if ((robot_collector_x + 1 == robot_sapper_x) && (robot_collector_y == robot_sapper_y)) {
					return false;
				}
			}
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		case movement::down: {
			if (context->map->get_robot_collector_block(0, -1) == block::rock) {
				return false;
			}
			if (sapper_exist == true) {
				if ((robot_collector_x == robot_sapper_x) && (robot_collector_y - 1 == robot_sapper_y)) {
					return false;
				}
			}
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		case movement::left: {
			if (context->map->get_robot_collector_block(-1, 0) == block::rock) {
				return false;
			}
			if (sapper_exist == true) {
				if ((robot_collector_x - 1 == robot_sapper_x) && (robot_collector_y == robot_sapper_y)) {
					return false;
				}
			}
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		default: {	return false; }
	}

	context->map->update_robot_collector_existence();
	//this->scan(context);

	return true;
}
void Manual_Controller::move_sapper(Context * context, movement m) {
	return;
}

void Manual_Controller::grab(Context * context) {
	block b = context->map->get_robot_collector_block(0, 0);
	if (b == block::apple) {
		context->map->grab();
		context->RC->grab();
	}
	return;
}
void Manual_Controller::scan(Context * context, Renderer_Handler* renderer_handler, int N) {
	return;
}
void Manual_Controller::scan(Context* context) {
	this->scaner->scanning(context->RC, context->map);
}
void Manual_Controller::auto_grab(Context * context, Renderer_Handler* renderer_handler) {
	return;
}

std::vector<std::vector<block>> Manual_Controller::get_render_map(Context * context, int render_length, int render_width, int robot_screen_x, int robot_screen_y) {
	Robot_Playground* rpg = context->RC->get_map();
	std::vector<std::vector<block>> robot_map = rpg->get_renderer_map();
	std::pair<int, int> shift = rpg->get_shift();
	std::pair<int, int> rc_coords = context->RC->get_coord_on_his_own_map();
	int length = rpg->get_length();
	int width = rpg->get_width();
	std::vector<std::vector<block>> result(render_length, std::vector<block>(render_width, block::unknown));

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
	if (context->map->robot_sapper_exist() == true && context->RS != nullptr) {
		std::pair<int, int> rs_coords = context->RS->get_coord_on_his_own_map();
		int sapper_x = rs_coords.first - rc_coords.first + robot_screen_x;
		int sapper_y = rs_coords.second - rc_coords.second + robot_screen_y;
		if ((sapper_x >= 0) && (sapper_x <= (render_length - 1)) && (sapper_y >= 0) && (sapper_y <= (render_width - 1))) {
			result[sapper_x][sapper_y] = block::robot_sapper;
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

//----------------------------------------------------------------------------
//----------------------Scan_Controller methods-------------------------------
//----------------------------------------------------------------------------

movement rotate_right(movement move) {
	switch (move) {
		case movement::up: {	return movement::right;	}
		case movement::right: {	return movement::down;	}
		case movement::down: {	return movement::left;	}
		case movement::left: {	return movement::up;	}
		default: {				return movement::up; }
	}
}
movement rotate_left(movement move) {
	switch (move) {
	case movement::up: {	return movement::left;	}
	case movement::left: {	return movement::down;	}
	case movement::down: {	return movement::right;	}
	case movement::right: {	return movement::up;	}
	default: {				return movement::up; }
	}
}

void Scan_Controller::sapper_on(Context * context) {
	return;
}
void Scan_Controller::sapper_off(Context * context) {
	return;
}
bool Scan_Controller::check_move(Context* context, movement m) {
		switch (m) {
		case movement::left:{ 

			if (context->RS != nullptr) {
				std::pair<int, int> robot_collector_coords = context->RC->get_coord_on_his_own_map();
				std::pair<int, int> robot_sapper_coords = context->RS->get_coord_on_his_own_map();
				if (robot_collector_coords.first - 1 == robot_sapper_coords.first && robot_collector_coords.second == robot_sapper_coords.second) {
					return false;
				}
			}

			block b = context->map->get_robot_collector_block(-1, 0);// ->get_data(robot_coords.first - 1, robot_coords.second);
			return (b != bomb && b != rock); 
		}

		case movement::up: {

			if (context->RS != nullptr) {
				std::pair<int, int> robot_collector_coords = context->RC->get_coord_on_his_own_map();
				std::pair<int, int> robot_sapper_coords = context->RS->get_coord_on_his_own_map();
				if (robot_collector_coords.first == robot_sapper_coords.first && robot_collector_coords.second + 1 == robot_sapper_coords.second) {
					return false;
				}
			}

			block b = context->map->get_robot_collector_block(0, 1);
			return (b != bomb && b != rock);
		}

		case movement::right: {

			if (context->RS != nullptr) {
				std::pair<int, int> robot_collector_coords = context->RC->get_coord_on_his_own_map();
				std::pair<int, int> robot_sapper_coords = context->RS->get_coord_on_his_own_map();
				if (robot_collector_coords.first + 1 == robot_sapper_coords.first && robot_collector_coords.second == robot_sapper_coords.second) {
					return false;
				}
			}

			block b = context->map->get_robot_collector_block(1, 0);
			return (b != bomb && b != rock);
		}

		case movement::down: {

			if (context->RS != nullptr) {
				std::pair<int, int> robot_collector_coords = context->RC->get_coord_on_his_own_map();
				std::pair<int, int> robot_sapper_coords = context->RS->get_coord_on_his_own_map();
				if (robot_collector_coords.first == robot_sapper_coords.first && robot_collector_coords.second - 1 == robot_sapper_coords.second) {
					return false;
				}
			}

			block b = context->map->get_robot_collector_block(0, -1);
			return (b != bomb && b != rock);
		}
		default: {return false; }
	}
	return true;
}
bool Scan_Controller::move_collector(Context * context, movement m) {
	switch (m) {
		case movement::up: {
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		case movement::right: {
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		case movement::down: {
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		case movement::left: {
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		default: {	return false; }
	}

	context->map->update_robot_collector_existence();

	return true;
}
void Scan_Controller::move_sapper(Context * context, movement m) {
	return;
}
void Scan_Controller::grab(Context * context) {
	return;
}
void Scan_Controller::scan(Context * context, Renderer_Handler* renderer_handler, int N) {
	int counter = 0;
	if (!this->check_move(context, movement::up) && !this->check_move(context, movement::right) && !this->check_move(context, movement::down) && !this->check_move(context, movement::left)) {
		return;
	}
	Drawing_Area da = renderer_handler->da;
	movement def = movement::up;

	if (!this->check_move(context, movement::up)) {
		def = movement::left;
	}
	else if (!this->check_move(context, movement::left)) {
		def = movement::down;
	}
	else if (!this->check_move(context, movement::down)) {
		def = movement::right;
	}
	else if (!this->check_move(context, movement::right)) {
		def = movement::up;
	}
	else {
		while (counter < N && this->check_move(context, def)) {
			this->move_collector(context, def);
			this->scan(context);
			SDL_Delay(50);
			renderer_handler->move_background(def);
			renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
			++counter;
		}
		def = rotate_left(def);
	}
	

	while (counter < N) {
		if (!this->check_move(context, rotate_right(def))) {
			if (!this->check_move(context, def)) {
				if (!this->check_move(context, rotate_left(def))) {
					def = rotate_left(def);
					def = rotate_left(def);
					this->move_collector(context, def);
					this->scan(context);
					SDL_Delay(50);
					renderer_handler->move_background(def);
					renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
					++counter;
					continue;
				}
				else {
					def = rotate_left(def);
					this->move_collector(context, def);
					this->scan(context);
					SDL_Delay(50);
					renderer_handler->move_background(def);
					renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
					++counter;
					continue;
				}
			}
			else {
				this->move_collector(context, def);
				this->scan(context);
				SDL_Delay(50);
				renderer_handler->move_background(def);
				renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
				++counter;
				continue;
			}
		}
		else {
			def = rotate_right(def);
			this->move_collector(context, def);
			this->scan(context);
			SDL_Delay(50);
			renderer_handler->move_background(def);
			renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
			++counter;
			continue;
		}

	}
	return;
}
void Scan_Controller::scan(Context* context) {
	this->scaner->scanning(context->RC, context->map);
}
void Scan_Controller::auto_grab(Context * context, Renderer_Handler* renderer_handler) {
	return;
}

std::vector<std::vector<block>> Scan_Controller::get_render_map(Context * context, int render_length, int render_width, int robot_screen_x, int robot_screen_y) {
	Robot_Playground* rpg = context->RC->get_map();
	std::vector<std::vector<block>> robot_map = rpg->get_renderer_map();
	std::pair<int, int> shift = rpg->get_shift();
	std::pair<int, int> rc_coords = context->RC->get_coord_on_his_own_map();
	int length = rpg->get_length();
	int width = rpg->get_width();
	std::vector<std::vector<block>> result(render_length, std::vector<block>(render_width, block::unknown));

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

	if (context->map->robot_sapper_exist() == true && context->RS != nullptr) {
		std::pair<int, int> rs_coords = context->RS->get_coord_on_his_own_map();
		int sapper_x = rs_coords.first - rc_coords.first + robot_screen_x;
		int sapper_y = rs_coords.second - rc_coords.second + robot_screen_y;
		if ((sapper_x >= 0) && (sapper_x <= (render_length - 1)) && (sapper_y >= 0) && (sapper_y <= (render_width - 1))) {
			result[sapper_x][sapper_y] = block::robot_sapper;
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


//----------------------------------------------------------------------------
//----------------------Auto_Controller methods-------------------------------
//----------------------------------------------------------------------------

void Auto_Controller::sapper_on(Context * context) {
	return;
}
void Auto_Controller::sapper_off(Context * context) {
	return;
}
void Auto_Controller::demine(Context * context) {
	context->RS->demine();
	context->map->demine();
	std::pair<int, int> rs_coords = context->RS->get_coord_on_his_own_map();
	context->RC->get_map()->put(rs_coords.first, rs_coords.second, block::empty);
	return;
}

typedef
struct node {
	int x, y;
	movement unmove;
	node* prev;
	node(int x, int y, movement m = movement(5), node* prev = nullptr) : x(x), y(y), unmove(m), prev(prev) {	};
	~node() {
		if (this->prev != nullptr) {
			delete this->prev;
		}
	}
} node;

bool Auto_Controller::check_collector_move(std::vector<std::vector<block>> map, int x, int y, int rs_x, int rs_y, int length, int width, bool sapper_exist) {
	if (x < 0 || x > (length - 1) || y < 0 || y > (width - 1)) {
		return false;
	}
	if (map[x][y] == block::rock || map[x][y] == block::bomb || map[x][y] == block::unknown) {
		return false;
	}
	if (sapper_exist == false) {
		return true;
	}
	if ((x == rs_x) && (y == rs_y)) {
		return false;
	}
	return true;
}
bool Auto_Controller::check_sapper_move(std::vector<std::vector<block>> map, int x, int y, int rc_x, int rc_y, int length, int width) {
	if (x < 0 || x > length - 1 || y < 0 || y > width - 1) {
		return false;
	}
	if (map[x][y] == block::rock || map[x][y] == block::unknown) {
		return false;
	}
	if ((x == rc_x) && (y == rc_y)) {
		return false;
	}
	return true;
}

std::vector<movement> Auto_Controller::find_way_collector(Context * context) {
	Robot_Playground* rpg = context->RC->get_map();
	std::pair<int, int> robot_collector_coords = context->RC->get_coord_on_his_own_map();

	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	std::pair<int, int> shift = rpg->get_shift();
	int length = rpg->get_length();
	int width = rpg->get_width();
	bool sapper_exist = context->map->robot_sapper_exist();

	std::vector<std::vector<bool>> check(length, std::vector<bool>(width, true));
	int robot_sapper_x = 0, robot_sapper_y = 0;
	if (context->RS != nullptr) {
		std::pair<int, int> robot_sapper_coords = context->RS->get_coord_on_his_own_map();
		robot_sapper_x = robot_sapper_coords.first + shift.first;
		robot_sapper_y = robot_sapper_coords.second + shift.second;
	}


	node * begin = new node(robot_collector_coords.first + shift.first, robot_collector_coords.second + shift.second);
	check[robot_collector_coords.first + shift.first][robot_collector_coords.second + shift.second] = false;

	node * result = nullptr;
	std::queue<node*>* q = new std::queue<node*>();
	q->push(begin);
	while (!q->empty()) {
		node * n = (q->front());
		if (pg[n->x][n->y] == block::apple) {
			result = n;// new node(n->x, n->y, n->unmove, n->prev);
			break;
		}
		q->pop();
		if (check_collector_move(pg, n->x, n->y + 1, robot_sapper_x, robot_sapper_y, length, width, sapper_exist) == true && check[n->x][n->y + 1] == true) {
			node * tmp = new node(n->x, n->y + 1, movement::up, n);
			check[n->x][n->y + 1] = false;
			q->push(tmp);
		}
		if (check_collector_move(pg, n->x + 1, n->y, robot_sapper_x, robot_sapper_y, length, width, sapper_exist) == true && check[n->x + 1][n->y] == true) {
			node * tmp = new node(n->x + 1, n->y, movement::right, n);
			check[n->x + 1][n->y] = false;
			q->push(tmp);
		}
		if (check_collector_move(pg, n->x, n->y - 1, robot_sapper_x, robot_sapper_y, length, width, sapper_exist) == true && check[n->x][n->y - 1] == true) {
			node * tmp = new node(n->x, n->y - 1, movement::down, n);
			check[n->x][n->y - 1] = false;
			q->push(tmp);
		}
		if (check_collector_move(pg, n->x - 1, n->y, robot_sapper_x, robot_sapper_y, length, width, sapper_exist) == true && check[n->x - 1][n->y] == true) {
			node * tmp = new node(n->x - 1, n->y, movement::left, n);
			check[n->x - 1][n->y] = false;
			q->push(tmp);
		}
	}
	std::vector<movement> way = {};
	while (result != nullptr && result->prev != nullptr) {
		way.push_back(result->unmove);
		result = result->prev;
	}

	std::vector<movement> result2(way.size());
	for (int i = 0; i < way.size(); ++i) {
		result2[i] = way[way.size() - 1 - i];
	}
	delete q;
	delete result;
	return result2;
}
std::vector<movement> Auto_Controller::find_way_sapper(Context * context) {
	Robot_Playground* rpg = context->RS->get_map();
	std::pair<int, int> robot_sapper_coords = context->RS->get_coord_on_his_own_map();
	std::pair<int, int> robot_collector_coords = context->RC->get_coord_on_his_own_map();
	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	std::pair<int, int> shift = rpg->get_shift();
	int length = rpg->get_length();
	int width = rpg->get_width();
	int robot_collector_x = robot_collector_coords.first + shift.first;
	int robot_collector_y = robot_collector_coords.second + shift.second;


	std::vector<std::vector<bool>> check(length, std::vector<bool>(width, true));

	node * begin = new node(robot_sapper_coords.first + shift.first, robot_sapper_coords.second + shift.second);
	check[robot_sapper_coords.first + shift.first][robot_sapper_coords.second + shift.second] = false;

	node * result = nullptr;
	std::queue<node*>* q = new std::queue<node*>();
	q->push(begin);
	while (!q->empty()) {
		node * n = (q->front());
		if (pg[n->x][n->y] == block::bomb) {
			result = n;// new node(n->x, n->y, n->unmove, n->prev);
			break;
		}
		q->pop();
		if (this->check_sapper_move(pg, n->x, n->y + 1, robot_collector_x, robot_collector_y, length, width) == true && check[n->x][n->y + 1] == true) {
			node * tmp = new node(n->x, n->y + 1, movement::up, n);
			check[n->x][n->y + 1] = false;
			q->push(tmp);
		}
		if (this->check_sapper_move(pg, n->x + 1, n->y, robot_collector_x, robot_collector_y, length, width) == true && check[n->x + 1][n->y] == true) {
			node * tmp = new node(n->x + 1, n->y, movement::right, n);
			check[n->x + 1][n->y] = false;
			q->push(tmp);
		}
		if (this->check_sapper_move(pg, n->x, n->y - 1, robot_collector_x, robot_collector_y, length, width) == true && check[n->x][n->y - 1] == true) {
			node * tmp = new node(n->x, n->y - 1, movement::down, n);
			check[n->x][n->y - 1] = false;
			q->push(tmp);
		}
		if (this->check_sapper_move(pg, n->x - 1, n->y, robot_collector_x, robot_collector_y, length, width) == true && check[n->x - 1][n->y] == true) {
			node * tmp = new node(n->x - 1, n->y, movement::left, n);
			check[n->x - 1][n->y] = false;
			q->push(tmp);
		}
	}
	std::vector<movement> way = {};
	while (result != nullptr && result->prev != nullptr) {
		way.push_back(result->unmove);
		result = result->prev;
	}

	std::vector<movement> result2(way.size());
	for (int i = 0; i < way.size(); ++i) {
		result2[i] = way[way.size() - 1 - i];
	}

	delete q;
	delete result;
	return result2;
}

bool Auto_Controller::apples_on_map(Context * context) {
	Robot_Playground* rpg = context->RC->get_map();
	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	int length = rpg->get_length();
	int width = rpg->get_width();

	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < width; ++j) {
			if (pg[i][j] == block::apple) {
				return true;
			}
		}
	}
	return false;
}
bool Auto_Controller::bombs_on_map(Context * context) {
	Robot_Playground* rpg = context->RS->get_map();
	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	int length = rpg->get_length();
	int width = rpg->get_width();

	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < width; ++j) {
			if (pg[i][j] == block::bomb) {
				return true;
			}
		}
	}
	return false;
}

bool Auto_Controller::move_collector(Context * context, movement m) {
	switch (m) {
		case movement::up:
		case movement::right:
		case movement::down: 
		case movement::left: {
			context->RC->move(m);
			context->map->move_robot_collector(m);
			break;
		}
		default: {	return false; }
	}
	return true;
}
void Auto_Controller::move_sapper(Context * context, movement m) {
	if (context->RS == nullptr) {
		return;
	}
	switch (m) {
		case movement::up: 
		case movement::right:
		case movement::down:
		case movement::left: {
			context->RS->move(m);
			context->map->move_robot_sapper(m);
			break;
		}
		default: {	return; }
	}
	return;
}

void Auto_Controller::grab(Context * context) {
	block b = context->map->get_robot_collector_block(0, 0);
	if (b == block::apple) {
		context->map->grab();
		context->RC->grab();
	}
	return;
}
void Auto_Controller::scan(Context * context, Renderer_Handler* renderer_handler, int N) {
	return;
}
void Auto_Controller::scan(Context * context) {
	return;
}
//fix phase effect
void Auto_Controller::auto_grab(Context * context, Renderer_Handler* renderer_handler) {
	Drawing_Area da = renderer_handler->da;
	if (context->map->get_robot_collector_block(0, 0) == block::apple) {
		this->grab(context);
		SDL_Delay(50);
		renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
	}
	while (this->apples_on_map(context)) {
		std::vector<movement> way = this->find_way_collector(context);
		if (way.size() == 0) {
			break;
		}
		for (int i = 0; i < way.size(); ++i) {
			this->move_collector(context, way[i]);
			SDL_Delay(50);
			renderer_handler->move_background(way[i]);
			renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
		}
		this->grab(context);
		SDL_Delay(50);
		renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
	}
	if (context->RS == nullptr) {
		return;
	}
	if (context->RS->get_map()->get_data(context->RS->get_coord_on_his_own_map().first, context->RS->get_coord_on_his_own_map().second) == block::bomb) {
		this->demine(context);
		SDL_Delay(50);
		renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
	}
	context->RS->update_map(context->RC->get_map());
	while (this->bombs_on_map(context)) {
		std::vector<movement> way = this->find_way_sapper(context);
		if (way.size() == 0) {
			break;
		}
		for (int i = 0; i < way.size(); ++i) {
			this->move_sapper(context, way[i]);
			SDL_Delay(50);
			//renderer_handler->move_background(way[i]);
			renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));

		}
		this->demine(context);
			SDL_Delay(50);
		renderer_handler->Update_Render(this->get_render_map(context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
	}
	return;
}

std::vector<std::vector<block>> Auto_Controller::get_render_map(Context * context, int render_length, int render_width, int robot_screen_x, int robot_screen_y) {
	Robot_Playground* rpg = context->RC->get_map();
	std::vector<std::vector<block>> robot_map = rpg->get_renderer_map();
	std::pair<int, int> shift = rpg->get_shift();
	std::pair<int, int> rc_coords = context->RC->get_coord_on_his_own_map();
	int length = rpg->get_length();
	int width = rpg->get_width();
	std::vector<std::vector<block>> result(render_length, std::vector<block>(render_width, block::unknown));

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

	if ((context->map->robot_sapper_exist() == true) && (context->RS != nullptr)) {
		std::pair<int, int> rs_coords = context->RS->get_coord_on_his_own_map();
		int sapper_x = rs_coords.first - rc_coords.first + robot_screen_x;
		int sapper_y = rs_coords.second - rc_coords.second + robot_screen_y;
		if ((sapper_x >= 0) && (sapper_x <= (render_length - 1)) && (sapper_y >= 0) && (sapper_y <= (render_width - 1))) {
			result[sapper_x][sapper_y] = block::robot_sapper;
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

//----------------------------------------------------------------------------
//-------------------------------SCANNER--------------------------------------
//----------------------------------------------------------------------------
void Scaner::scanning(Robot_Collector* RC, Main_map* main_map) {
		std::pair<int, int> rc_coords = RC->get_coord_on_his_own_map();
		Robot_Playground* rpg = RC->get_map();
		// whatever you want
		block b = main_map->get_robot_collector_block(0, 0);
		rpg->put(rc_coords.first, rc_coords.second, b);
		b = main_map->get_robot_collector_block(0, 1);
		rpg->put(rc_coords.first, rc_coords.second + 1, b);
		b = main_map->get_robot_collector_block(1, 0);
		rpg->put(rc_coords.first + 1, rc_coords.second, b);
		b = main_map->get_robot_collector_block(0, -1);
		rpg->put(rc_coords.first, rc_coords.second - 1, b);
		b = main_map->get_robot_collector_block(-1, 0);
		rpg->put(rc_coords.first - 1, rc_coords.second, b);
		// end of whatever you want
		return;
	}
