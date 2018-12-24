#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <exception>
#include <vector>
#include <map>

#include "Game.h"
#include "Robot.h"
#include "Text_Sprite.h"
#include "test.cpp"
#include "Controller.h"

using namespace Game;
using namespace std;
using namespace Robots;

struct Drawing_Area {
	int right_border;
	int left_border;
	int top_border;
	int bot_border;
	int center_x;
	int center_y;
	Drawing_Area(
		int right_border,
		int left_border,
		int top_border,
		int bot_border,
		int center_x,
		int center_y) :
		right_border(right_border),
		left_border(left_border),
		top_border(top_border),
		bot_border(bot_border),
		center_x(center_x),
		center_y(center_y) {}
	Drawing_Area() {}
};



class Main_Controller : public Game::Sprite_Controller {
	Controller* controller = new Manual_Controller();
	Context* context;
	Sprite* robot_collector;
	Sprite* background;
	Text_Sprite* scores;
	Drawing_Area da;
	Uint8 right = 0;
	Uint8 left = 0;
	Uint8 up = 0;
	Uint8 down = 0;

	vector<pair<Uint8, char>> num_keys = {
		pair<Uint8, char>(SDL_SCANCODE_0, 0),
		pair<Uint8, char>(SDL_SCANCODE_1, 1),
		pair<Uint8, char>(SDL_SCANCODE_2, 2),
		pair<Uint8, char>(SDL_SCANCODE_3, 3),
		pair<Uint8, char>(SDL_SCANCODE_4, 4),
		pair<Uint8, char>(SDL_SCANCODE_5, 5),
		pair<Uint8, char>(SDL_SCANCODE_6, 6),
		pair<Uint8, char>(SDL_SCANCODE_7, 7),
		pair<Uint8, char>(SDL_SCANCODE_8, 8),
		pair<Uint8, char>(SDL_SCANCODE_9, 9)
	};

public:
	Main_Controller(Sprite* robot_collector, Sprite* background, Drawing_Area da, Controller* controller, Context* context, Text_Sprite* scores) {
		this->robot_collector = robot_collector;
		this->background = background;
		this->da = da;
		this->controller = controller;
		this->context = context;
		this->scores = scores;
	}

	std::vector<std::vector<block>> handle_sprites() {
		const Uint8* key_state = SDL_GetKeyboardState(NULL);
		if (key_state[SDL_SCANCODE_N]) {
			unsigned int n = check_command(); // тут можно вытащить введённое число
			cout << n << endl;
		}

		if (key_state[SDL_SCANCODE_G]) {
			this->controller->grab(this->context);
		}

		scores->Change_Text(to_string(this->context->RC->get_apples()).data());
		//std::vector<std::vector<block>> hz(5, std::vector<block>(5, block::empty));

		controll_robot_collector(key_state);
		return this->controller->get_render_map(this->context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size);
		//return robot_controller->render_map(da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size );
	}

	int get_only_digit(char ch) {
		char digits[] = {
			'0', '1', '2', '3', '4',
			'5', '6', '7', '8', '9'
		};
		for (int i = 0; i < 10; i++) {
			if (digits[i] == ch) {
				return i;
			}
		}
		return -1;
	}

	inline unsigned int check_command() {
		unsigned int n = 0;

		bool is_return = false;
		SDL_Event Event;

		while (!is_return) {
			while (SDL_PollEvent(&Event)) {
				SDL_PumpEvents();
				const Uint8* t_key_state = SDL_GetKeyboardState(NULL);
				if (t_key_state[SDL_SCANCODE_RETURN]) {
					is_return = true;
					return n;
				}
				int digit = get_only_digit(Event.text.text[0]);
				if (digit != -1)
					n = n * 10 + digit;
			}
		}
		return n;
	}

	inline void controll_robot_collector(const Uint8* key_state) {
		if (key_state[SDL_SCANCODE_RIGHT] && !right) {
			this->controller->move_collector(this->context, movement::right);
			this->controller->scan(this->context);
			background->rect.x -= sprite_size;
			if (robot_collector->flip != SDL_FLIP_NONE) {
				robot_collector->flip = SDL_FLIP_NONE;
			}
		}
		else if (key_state[SDL_SCANCODE_LEFT] && !left) {
			this->controller->move_collector(this->context, movement::left);
			this->controller->scan(this->context);
			background->rect.x += sprite_size;
			if (robot_collector->flip != SDL_FLIP_HORIZONTAL) {
				robot_collector->flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (key_state[SDL_SCANCODE_UP] && !up) {
			this->controller->move_collector(this->context, movement::up);
			this->controller->scan(this->context);
			background->rect.y += sprite_size;
		}
		else if (key_state[SDL_SCANCODE_DOWN] && !down) {
			this->controller->move_collector(this->context, movement::down);
			this->controller->scan(this->context);
			background->rect.y -= sprite_size;
		}
		if (key_state[SDL_SCANCODE_ESCAPE]) {
			exit(0);
		}

		right = key_state[SDL_SCANCODE_RIGHT];
		left = key_state[SDL_SCANCODE_LEFT];
		up = key_state[SDL_SCANCODE_UP];
		down = key_state[SDL_SCANCODE_DOWN];
	}
};

int main(int argc, char** argv) {
	Game_Handler g;

	SDL_DisplayMode display_mode;
	int req = SDL_GetDesktopDisplayMode(0, &display_mode);
	if (req != 0) {
		std::cout << "SDL_Init_error: " << SDL_GetError() << std::endl;
		throw Game_Exception("SDL_Init_error");
	}

	Window_Properties wp("LOL", 0, 0, display_mode.w, display_mode.h, SDL_WINDOW_SHOWN /*| SDL_WINDOW_FULLSCREEN_DESKTOP*/);
	//Window_Properties wp("LOL", 50, 50, 1000, 800, SDL_WINDOW_SHOWN);

	g.Create_Window(wp);
	g.Create_Renderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Sprite* background = new Sprite("background.png", SDL_Rect_(-512 * sprite_size, -512 * sprite_size, 1024 * sprite_size, 1024 * sprite_size), visible);

	Drawing_Area da(
		wp.w + sprite_size - (wp.w % sprite_size) / 2, // right
		-(wp.w % sprite_size) / 2,						// left
		wp.h + sprite_size - (wp.w % sprite_size) / 2,	// top
		-(wp.h % sprite_size) / 2,						// bot
		(wp.w + sprite_size)/2 - (((wp.w + sprite_size) / 2) % sprite_size) - (wp.w % sprite_size) / 2, // center x
		(wp.h + sprite_size)/2 - (((wp.h + sprite_size) / 2) % sprite_size) - (wp.w % sprite_size) / 2); // center y

	Sprite* robot_collector = new Sprite("robot1.PNG", SDL_Rect_(da.center_x, da.center_y, sprite_size, sprite_size), visible);

	Sprite* apple = new Sprite("apple.PNG", SDL_Rect_(da.left_border, da.bot_border, sprite_size, sprite_size), visible);
	Sprite* bomb = new Sprite("bomb.PNG", SDL_Rect_(da.left_border, da.bot_border, sprite_size, sprite_size), visible);
	Sprite* rock = new Sprite("rock.PNG", SDL_Rect_(da.left_border, da.bot_border, sprite_size, sprite_size), visible);
	Sprite* unknown = new Sprite("unknown.PNG", SDL_Rect_(da.left_border, da.bot_border, sprite_size, sprite_size), visible);
	//FILE * mappp = fopen("map1.txt", "r");

	//Robot_Controller* robot_controller = new Robot_Controller();
	Controller* controller = new Manual_Controller();
	Context* context = new Context("map1.txt");
	controller->scan(context);
	//robot_controller->main_map = new Main_map("map1.txt");
	//robot_controller->create_RC();
	//robot_controller->main_map->create_robot_collector();
	//robot_controller->RC->scan(robot_controller->main_map->get_robot_collector_neibourhood());
	//robot_controller->scan(2);

	TTF_Init();

	Text_Sprite* scores = new Text_Sprite("Hello", 100, SDL_Color_(0, 0, 0), SDL_Rect_(da.center_x, 0, 100, 100));

	Game::Sprite_Controller* main_controller = new Main_Controller(robot_collector, background, da, controller, context, scores);
	g.Set_Sprites(robot_collector, apple, rock, unknown, bomb, background, scores);
	g.mainloop(main_controller);

	TTF_Quit();
	delete main_controller;
	delete robot_collector, apple, bomb, rock, unknown, controller, context, scores;

	return 0;
}
