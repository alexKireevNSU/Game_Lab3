#include "enums.h"
#include "Robot.h"
#include "Main_map.h"
#include "Context.h"
#include "Controller.h"

//----------------------------------------------------------------------------
//----------------------Manual_Controller methods-----------------------------
//----------------------------------------------------------------------------

void Manual_Controller::move_collector(Context* context, movement m) {

	//dadad
	//context->RC->scan(context->map->get_robot_collector_block(0, 0);
	//net net
	switch (m) {
		case movement::up: {
			if (context->map->get_robot_collector_block(0, 1) == block::rock) {
				return;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
				break;
			}
		}
		case movement::right: {
			if (context->map->get_robot_collector_block(1, 0) == block::rock) {
				return;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
				break;
			}
		}
		case movement::down: {
			if (context->map->get_robot_collector_block(0, -1) == block::rock) {
				return;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
				break;
			}
		}
		case movement::left: {
			if (context->map->get_robot_collector_block(-1, 0) == block::rock) {
				return;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
				break;
			}
		}
		default: {	return; }
	}

	//this->scan(context);

	return;
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

void Manual_Controller::scan(Context * context, int N) {
	return;
}

void Manual_Controller::scan(Context* context) {
	this->scaner->scanning(context->RC, context->map);
}

void Manual_Controller::auto_grab(Context * context) {
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

void Scan_Controller::move_collector(Context * context, movement m) {
	return;
}

void Scan_Controller::move_sapper(Context * context, movement m) {
	return;
}

void Scan_Controller::grab(Context * context) {
	return;
}

void Scan_Controller::scan(Context * context, int N) {
	//auto_scan N func;
}

void Scan_Controller::scan(Context* context) {
	this->scaner->scanning(context->RC, context->map);
}

void Scan_Controller::auto_grab(Context * context) {
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

void Auto_Controller::demine(Context * context) {

}


void Auto_Controller::find_way(Context * context) {

}

void Auto_Controller::move_collector(Context * context, movement m) {
	switch (m) {
		case movement::up: {
			block b = context->map->get_robot_collector_block(0, 1);
			if (b == block::rock || b == block::bomb) {
				return;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
			}
		}
		case movement::right: {
			block b = context->map->get_robot_collector_block(1, 0);
			if (b == block::rock || b == block::bomb) {
				return;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
			}
		}
		case movement::down: {
			block b = context->map->get_robot_collector_block(0, -1);
			if (b == block::rock || b == block::bomb) {
				return;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
			}
		}
		case movement::left: {
			block b = context->map->get_robot_collector_block(-1, 0);
			if (b == block::rock || b == block::bomb) {
				return;
			}
			else {
				context->RC->move(m);
				context->map->move_robot_collector(m);
			}
		}
		default: {	return; }
	}
	return;
}

void Auto_Controller::move_sapper(Context * context, movement m) {
	switch (m) {
		case movement::up: {
			block b = context->RS->get_block(0, 1);
			if (b == block::rock || b == block::unknown) {
				return;
			}
			else {
				context->RS->move(m);
				context->map->move_robot_sapper(m);
			}
		}
		case movement::right: {
			block b = context->RS->get_block(1, 0);
			if (b == block::rock || b == block::unknown) {
				return;
			}
			else {
				context->RS->move(m);
				context->map->move_robot_sapper(m);
			}
		}
		case movement::down: {
			block b = context->RS->get_block(0, -1);
			if (b == block::rock || b == block::unknown) {
				return;
			}
			else {
				context->RS->move(m);
				context->map->move_robot_sapper(m);
			}
		}
		case movement::left: {
			block b = context->RS->get_block(-1, 0);
			if (b == block::rock || b == block::unknown) {
				return;
			}
			else {
				context->RS->move(m);
				context->map->move_robot_sapper(m);
			}
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

void Auto_Controller::scan(Context * context, int N) {
	return;
}

void Auto_Controller::auto_grab(Context * context) {

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
