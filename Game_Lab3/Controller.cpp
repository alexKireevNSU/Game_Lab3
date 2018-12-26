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
			if (pg[i][j] != block::unknown && pg[i][j] != block::rock && i != rc_x && j != rc_y) {
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
		//context->map->update_robot_sapper_existence(true);
	}
	Robot_Playground* rpg = context->RS->get_map();
	std::pair<int, int> robot_coords = context->RS->get_coord_on_his_own_map();
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
	for (int i = 0; i < length; ++i) {
		for (int j = 0; j < width; ++j) {
			if (pg[i][j] != block::unknown && pg[i][j] != block::rock && i != rc_x && j != rc_y) {
				sapper_x = i;
				sapper_y = j;
				break;
			}
		}
	}
	sapper_x -= shift.first;
	sapper_y -= shift.second;
	context->RS = new Robot_Sapper(context->RC->get_map(), sapper_x, sapper_y, robot_coords.first, robot_coords.second);


	context->map->create_robot_sapper(0, 0);
}

void Manual_Controller::sapper_off(Context * context) {
	delete context->RS;
	context->map->update_robot_sapper_existence(false);
}

bool Manual_Controller::move_collector(Context* context, movement m) {
	switch (m) {
		case movement::up: {
			if (context->map->get_robot_collector_block(0, 1) == block::rock) {
				return false;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
				break;
			}
		}
		case movement::right: {
			if (context->map->get_robot_collector_block(1, 0) == block::rock) {
				return false;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
				break;
			}
		}
		case movement::down: {
			if (context->map->get_robot_collector_block(0, -1) == block::rock) {
				return false;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
				break;
			}
		}
		case movement::left: {
			if (context->map->get_robot_collector_block(-1, 0) == block::rock) {
				return false;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
				break;
			}
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

void Scan_Controller::sapper_on(Context * context) {
	return;
}

void Scan_Controller::sapper_off(Context * context) {
	return;
}

bool Scan_Controller::check_move(Context* context, movement m) {
	switch (m) {
		case movement::left:{ 
			//std::cout << "check_left" << std::endl;
			block b = context->map->get_robot_collector_block(-1, 0);// ->get_data(robot_coords.first - 1, robot_coords.second);
			return (b != bomb && b != rock); 
		}
		case movement::up: {
			//std::cout << "check_up" << std::endl;
			block b = context->map->get_robot_collector_block(0, 1);
			return (b != bomb && b != rock);
		}
		case movement::right: {
			//std::cout << "check_right" << std::endl;
			block b = context->map->get_robot_collector_block(1, 0);
			return (b != bomb && b != rock);
		}
		case movement::down: {
			//std::cout << "check_down" << std::endl;
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
	while (counter < N) {
		while (this->check_move(context, movement::up) && (counter < N)) {
			this->move_collector(context, movement::up);
			this->scan(context);
			++counter;
		}
		while (this->check_move(context, movement::right) && (counter < N)) {
			this->move_collector(context, movement::right);
			this->scan(context);
			++counter;
		}
		while (this->check_move(context, movement::down) && (counter < N)) {
			this->move_collector(context, movement::down);
			this->scan(context);
			++counter;
		}
		while (this->check_move(context, movement::left) && (counter < N)) {
			this->move_collector(context, movement::left);
			this->scan(context);
			//renderer_handler->Update_Render()
			++counter;
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

//dodelat'
void Auto_Controller::demine(Context * context) {
	context->RS->demine();
	context->map->demine();
	Robot_Playground* rpg = context->RC->get_map();
	//std::pair<int, int> = get
	//rpg->put()
}



struct node {
	int x, y;
	movement unmove;
	node* prev;
	node(int x, int y, movement m = movement(5), node* prev = nullptr) : x(x), y(y), unmove(m), prev(prev) {	};
};

bool Auto_Controller::check_collector_move(std::vector<std::vector<block>> map, int x, int y, int length, int width) {
	if (x < 0 || x > (length - 1) || y < 0 || y > (width - 1)) {
		return false;
	}
	if (map[x][y] == block::rock || map[x][y] == block::bomb || map[x][y] == block::unknown) {
		return false;
	}
	return true;
}

bool Auto_Controller::check_sapper_move(std::vector<std::vector<block>> map, int x, int y, int length, int width) {
	if (x < 0 || x > length - 1 || y < 0 || y > width - 1) {
		return false;
	}
	if (map[x][y] == block::rock || map[x][y] == block::unknown) {
		return false;
	}
	return true;
}

std::vector<movement> Auto_Controller::find_way_controller(Context * context) {
	if (context->map->get_robot_collector_block(0, 0) == block::apple) {
		return std::vector<movement> { };
	}

	Robot_Playground* rpg = context->RC->get_map();
	std::pair<int, int> robot_coords = context->RC->get_coord_on_his_own_map();
	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	std::pair<int, int> shift = rpg->get_shift();
	int length = rpg->get_length();
	int width = rpg->get_width();

	node * begin = new node(robot_coords.first + shift.first, robot_coords.second + shift.second);
	node * result;
	std::queue<node*>* q = new std::queue<node*>();
	q->push(begin);
	while (true) {
		node * n = (q->front());
		if (pg[n->x][n->y] == block::apple) {
			result = n;
			break;
		}
		q->pop();
		if (n->unmove != movement::down && this->check_collector_move(pg, n->x, n->y + 1, length, width) == true) {
			node * tmp = new node(n->x, n->y + 1, movement::up, n);
			q->push(tmp);
		}
		if (n->unmove != movement::left && this->check_collector_move(pg, n->x + 1, n->y, length, width) == true) {
			node * tmp = new node(n->x + 1, n->y, movement::right, n);
			q->push(tmp);
		}
		if (n->unmove != movement::up && this->check_collector_move(pg, n->x, n->y - 1, length, width) == true) {
			node * tmp = new node(n->x, n->y - 1, movement::down, n);
			q->push(tmp);
		}
		if (n->unmove != movement::right && this->check_collector_move(pg, n->x - 1, n->y, length, width) == true) {
			node * tmp = new node(n->x - 1, n->y, movement::left, n);
			q->push(tmp);
		}
	}
	std::vector<movement> way = {};
	while (result->prev != nullptr) {
		way.push_back(result->unmove);
		result = result->prev;
	}

	//way.reserve(way.size());
	
	//system("pause");
	std::vector<movement> result2(way.size());
	for (int i = 0; i < way.size(); ++i) {
		result2[i] = way[way.size() - 1 - i];
	}

	//std::cout << "________________________way___________________________" << std::endl;
	//for (int i = 0; i < result2.size(); ++i) {
	//	if (result2[i] == movement::up) std::cout << "up" << endl;
	//	if (result2[i] == movement::right) std::cout << "right" << endl;
	//	if (result2[i] == movement::down) std::cout << "down" << endl;
	//	if (result2[i] == movement::left) std::cout << "left" << endl;
	//}
	//std::cout << "________________________way___________________________" << std::endl << std::endl;

	delete q;
	delete result;
	//delete begin;
	return result2;
}

std::vector<movement> Auto_Controller::find_way_sapper(Context * context) {
	Robot_Playground* rpg = context->RS->get_map();
	std::pair<int, int> robot_coords = context->RS->get_coord_on_his_own_map();
	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	std::pair<int, int> shift = rpg->get_shift();
	int length = rpg->get_length();
	int width = rpg->get_width();

	node * begin = new node(robot_coords.first + shift.first, robot_coords.second + shift.second);
	node * result;
	std::queue<node*>* q = new std::queue<node*>();
	q->push(begin);
	while (true) {
		node * n = (q->front());
		if (pg[n->x][n->y] == block::apple) {
			result = n;
			break;
		}
		q->pop();
		//if (this->check_collector_move(pg, n->x, n->y + 1, length, width) == true) {
		//	node * tmp = new node(n->x, n->y + 1, movement::down, n);
		//	q->push(tmp);
		//}
		if (n->unmove != movement::down && this->check_sapper_move(pg, n->x, n->y + 1, length, width) == true) {
			node * tmp = new node(n->x, n->y + 1, movement::up, n);
			q->push(tmp);
		}
		//if (this->check_collector_move(pg, n->x + 1, n->y, length, width) == true) {
		//	node * tmp = new node(n->x + 1, n->y, movement::left, n);
		//	q->push(tmp);
		//}
		if (n->unmove != movement::left && this->check_sapper_move(pg, n->x + 1, n->y, length, width) == true) {
			node * tmp = new node(n->x + 1, n->y, movement::right, n);
			q->push(tmp);
		}
		//if (this->check_collector_move(pg, n->x, n->y - 1, length, width) == true) {
		//	node * tmp = new node(n->x, n->y - 1, movement::up, n);
		//	q->push(tmp);
		//}
		if (n->unmove != movement::up && this->check_sapper_move(pg, n->x, n->y - 1, length, width) == true) {
			node * tmp = new node(n->x, n->y - 1, movement::down, n);
			q->push(tmp);
		}
		//if (this->check_collector_move(pg, n->x - 1, n->y, length, width) == true) {
		//	node * tmp = new node(n->x - 1, n->y, movement::right, n);
		//	q->push(tmp);
		//}
		if (n->unmove != movement::right && this->check_sapper_move(pg, n->x - 1, n->y, length, width) == true) {
			node * tmp = new node(n->x - 1, n->y, movement::left, n);
			q->push(tmp);
		}
	}
	std::vector<movement> way = {};
	while (result->prev != nullptr) {
		way.push_back(result->unmove);
		result = result->prev;
	}
	//way.reserve(way.size());
	delete q;
	delete result;
	//delete begin;
	return way;
}

bool Auto_Controller::apples_on_map(Context * context) {
	Robot_Playground* rpg = context->RC->get_map();
	//std::pair<int, int> robot_coords = context->RC->get_coord_on_his_own_map();
	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	//std::pair<int, int> shift = rpg->get_shift();
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
	//std::pair<int, int> robot_coords = context->RC->get_coord_on_his_own_map();
	std::vector<std::vector<block>> pg = rpg->get_renderer_map();
	//std::pair<int, int> shift = rpg->get_shift();
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
	return true;
}

void Auto_Controller::move_sapper(Context * context, movement m) {
	if (context->RS == nullptr) {
		return;
	}
	switch (m) {
		case movement::up: {
			context->RS->move(m);
			context->map->move_robot_sapper(m);
			break;
		}
		case movement::right: {
			context->RS->move(m);
			context->map->move_robot_sapper(m);
			break;
		}
		case movement::down: {
			context->RS->move(m);
			context->map->move_robot_sapper(m);
			break;
		}
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

void Auto_Controller::auto_grab(Context * context, Renderer_Handler* renderer_handler) {
	while (this->apples_on_map(context)) {
		std::vector<movement> way = this->find_way_controller(context);
		for (int i = 0; i < way.size(); ++i) {
			this->move_collector(context, way[i]);
		}
		this->grab(context);
	}
	//while (this->bombs_on_map(context)) {
	//	std::vector<movement> way = this->find_way_sapper(context);
	//	for (int i = 0; i < way.size(); ++i) {
	//		this->move_sapper(context, way[i]);
	//	}
	//	this->demine(context);
	//}
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

	for (int i = 0; i < render_length; ++i) {
		for (int j = 0; j < render_width / 2; ++j) {
			block buff = result[i][j];
			result[i][j] = result[i][render_width - j - 1];
			result[i][render_width - j - 1] = buff;
		}
	}

	return result;
}

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
		//
		return;
	}
