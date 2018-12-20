#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <exception>
#include <vector>
#include <map>

#include "Game.h"
#include "Robot.h"
#include "test.cpp"

using namespace Game;
using namespace std;
using namespace Robots;

const int sprite_size = 75;

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
	Robot_Controller* robot_controller = new Robot_Controller();
	Sprite* robot_collector;
	Sprite* background;
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
	Main_Controller(Sprite* robot_collector, Sprite* background, Drawing_Area da, Robot_Controller* robot_controller) {
		this->robot_collector = robot_collector;
		this->background = background;
		this->da = da;
		this->robot_controller = robot_controller;
	}

	std::vector<std::vector<block>> handle_sprites() {
		const Uint8* key_state = SDL_GetKeyboardState(NULL);
		if (key_state[SDL_SCANCODE_N]) {
			unsigned int n = check_command(); // тут можно вытащить введённое число
			cout << n << endl;
		}

		//std::vector<std::vector<block>> hz(5, std::vector<block>(5, block::empty));

		controll_robot_collector(key_state);
		//cout << da.right_border / sprite_size << ' ' << da.top_border / sprite_size << endl;
		return robot_controller->render_map(da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size );
		//return hz;
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
			robot_controller->move_collector(movement::right);
			background->rect.x -= sprite_size;
			if (robot_collector->flip != SDL_FLIP_NONE) {
				robot_collector->flip = SDL_FLIP_NONE;
			}
		}
		else if (key_state[SDL_SCANCODE_LEFT] && !left) {
			robot_controller->move_collector(movement::left);
			background->rect.x += sprite_size;
			if (robot_collector->flip != SDL_FLIP_HORIZONTAL) {
				robot_collector->flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (key_state[SDL_SCANCODE_UP] && !up) {
			robot_controller->move_collector(movement::up);
			background->rect.y += sprite_size;
		}
		else if (key_state[SDL_SCANCODE_DOWN] && !down) {
			robot_controller->move_collector(movement::down);
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


	//void check_drawing_area_out() {
	//	if (robot_collector->rect.x + robot_collector->rect.w > da.right_border) {
	//		robot_collector->rect.x = da.left_border;
	//	}
	//	if (robot_collector->rect.x < da.left_border) {
	//		robot_collector->rect.x = da.right_border - robot_collector->rect.w;
	//	}
	//	if (robot_collector->rect.y + robot_collector->rect.h > da.top_border) {
	//		robot_collector->rect.y = da.bot_border;
	//	}
	//	if (robot_collector->rect.y < da.bot_border) {
	//		robot_collector->rect.y = da.top_border - robot_collector->rect.h;
	//	}
	//}
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
		(wp.w + sprite_size)/2 - (((wp.w + sprite_size) / 2) % sprite_size) - (wp.w % sprite_size) / 2,
		(wp.h + sprite_size)/2 - (((wp.h + sprite_size) / 2) % sprite_size) - (wp.w % sprite_size) / 2);

	//std::vector<Sprite*> sprites;
	/*int size = sprite_size;
	for (int i = da.bot_border; i < da.top_border; i += size) {
		for (int j = da.left_border; j < da.right_border; j += size) {
			sprites.push_back(new Sprite("jd.jpg", SDL_Rect_(j, i, size, size), visible));
		}
	}*/
	Sprite* robot_collector = new Sprite("robot1.PNG", SDL_Rect_(da.center_x, da.center_y, sprite_size, sprite_size), visible);

	Sprite* apple = new Sprite("apple.PNG", SDL_Rect_(da.left_border, da.bot_border, sprite_size, sprite_size), visible);
	Sprite* bomb = new Sprite("bomb.PNG", SDL_Rect_(da.left_border, da.bot_border, sprite_size, sprite_size), visible);
	Sprite* rock = new Sprite("rock.PNG", SDL_Rect_(da.left_border, da.bot_border, sprite_size, sprite_size), visible);
	Sprite* unknown = new Sprite("unknown.PNG", SDL_Rect_(da.left_border, da.bot_border, sprite_size, sprite_size), visible);
	//FILE * mappp = fopen("map1.txt", "r");
	Robot_Controller* robot_controller = new Robot_Controller();
	robot_controller->main_map = new Main_map();
	robot_controller->create_RC();

	Game::Sprite_Controller* main_controller = new Main_Controller(robot_collector, background, da, robot_controller);

	g.Set_Sprites(robot_collector, apple, rock, unknown, bomb, background);

	g.mainloop(main_controller);

	delete main_controller;


	return 0;
}
