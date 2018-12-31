#pragma once
#include "enums.h"
#include "Robot.h"
#include "Main_map.h"
#include "Context.h"
#include <vector>

using namespace Robots;

class Scaner {
public:
	void scanning(Robot_Collector* RC, Main_map* main_map);
};

class Controller {
public:
	virtual void sapper_on(Context * context) = 0;
	virtual void sapper_off(Context * context) = 0;
	virtual bool move_collector(Context * context, movement m) = 0;
	virtual void move_sapper(Context * context, movement m) = 0;
	virtual void grab(Context * context) = 0;
	virtual void scan(Context* context) = 0;
	virtual void scan(Context * context, Renderer_Handler* renderer_handler, int N) = 0;
	virtual void auto_grab(Context * context, Renderer_Handler* renderer_handler) = 0;
	virtual std::vector<std::vector<block>> get_render_map(Context * context, int length, int width, int robot_screen_x, int robot_screen_y) = 0;
};

class Manual_Controller : public Controller {
private:
	Scaner* scaner;
public:
	void sapper_on(Context * context);
	void sapper_off(Context * context);
	bool move_collector(Context * context, movement m);
	void move_sapper(Context * context, movement m);
	void grab(Context * context);
	void scan(Context* context);
	void scan(Context * context, Renderer_Handler* renderer_handler, int N);
	void auto_grab(Context * context, Renderer_Handler* renderer_handler);
	std::vector<std::vector<block>> get_render_map(Context * context, int length, int width, int robot_screen_x, int robot_screen_y);
};

class Scan_Controller : public Controller {
private:
	Scaner* scaner;
	bool check_move(Context* context, movement m);
public:
	void sapper_on(Context * context);
	void sapper_off(Context * context);
	bool move_collector(Context * context, movement m);
	void move_sapper(Context * context, movement m);
	void grab(Context * context);
	void scan(Context* context);
	void scan(Context * context, Renderer_Handler* renderer_handler, int N);
	void auto_grab(Context * context, Renderer_Handler* renderer_handler);
	std::vector<std::vector<block>> get_render_map(Context * context, int length, int width, int robot_screen_x, int robot_screen_y);
};

class Auto_Controller : public Controller {
private:
	Scaner* scaner;
	void demine(Context * context);
	std::vector<movement> find_way_controller(Context * context);
	std::vector<movement> find_way_sapper(Context * context);
	bool bombs_on_map(Context * context);
	bool apples_on_map(Context * context);
	//bool check_collector_move(std::vector<std::vector<block>> map, int x, int y, int length, int width);
	//bool check_sapper_move(std::vector<std::vector<block>> map, int x, int y, int length, int width);
public:
	//std::vector<movement> find_way_controller(Context * context);
	//bool apples_on_map(Context * context);
	void sapper_on(Context * context);
	void sapper_off(Context * context);
	bool move_collector(Context * context, movement m);
	void move_sapper(Context * context, movement m);
	void grab(Context * context);
	void scan(Context* context);
	void scan(Context * context, Renderer_Handler* renderer_handler, int N);
	void auto_grab(Context * context, Renderer_Handler* renderer_handler);
	std::vector<std::vector<block>> get_render_map(Context * context, int length, int width, int robot_screen_x, int robot_screen_y);
};