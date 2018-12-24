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
	//virtual Controller() = 0;
	virtual void move_collector(Context * context, movement m) = 0;
	virtual void move_sapper(Context * context, movement m) = 0;
	virtual void grab(Context * context) = 0;
	virtual void scan(Context* context) = 0;
	virtual void scan(Context * context, int N) = 0;
	virtual void auto_grab(Context * context) = 0;
	virtual std::vector<std::vector<block>> get_render_map(Context * context, int length, int width, int robot_screen_x, int robot_screen_y) = 0;
};

class Manual_Controller : public Controller {
private:
	Scaner* scaner;
public:
	void move_collector(Context * context, movement m);
	void move_sapper(Context * context, movement m);
	void grab(Context * context);
	void scan(Context* context);
	void scan(Context * context, int N);
	void auto_grab(Context * context);
	std::vector<std::vector<block>> get_render_map(Context * context, int length, int width, int robot_screen_x, int robot_screen_y);
};

class Scan_Controller : public Controller {
private:
	Scaner* scaner;
public:
	void move_collector(Context * context, movement m);
	void move_sapper(Context * context, movement m);
	void grab(Context * context);
	void scan(Context* context);
	void scan(Context * context, int N);
	void auto_grab(Context * context);
	std::vector<std::vector<block>> get_render_map(Context * context, int length, int width, int robot_screen_x, int robot_screen_y);
};

class Auto_Controller : public Controller {
private:
	Scaner* scaner;
	void demine(Context * context);
	void find_way(Context * context);
public:
	void move_collector(Context * context, movement m);
	void move_sapper(Context * context, movement m);
	void grab(Context * context);
	void scan(Context* context);
	void scan(Context * context, int N);
	void auto_grab(Context * context);
	std::vector<std::vector<block>> get_render_map(Context * context, int length, int width, int robot_screen_x, int robot_screen_y);
};