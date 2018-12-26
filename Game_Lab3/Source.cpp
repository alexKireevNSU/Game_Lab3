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

class Main_Controller : public Game::Sprite_Controller {
	Controller* controller = new Manual_Controller();
	Context* context;
	Sprite* robot_collector;
	Sprite* background;
	Sprite* death_screen;
	Text_Sprite* scores;
	Text_Sprite* autoscan_depth;
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
	Main_Controller(Sprite* robot_collector, Sprite* background, Drawing_Area da, Controller* controller, Context* context, Text_Sprite* scores, Text_Sprite* autoscan_depth, Sprite* death_screen) {
		this->robot_collector = robot_collector;
		this->background = background;
		this->da = da;
		this->controller = controller;
		this->context = context;
		this->scores = scores;
		this->autoscan_depth = autoscan_depth;
		this->death_screen = death_screen;
	}

	void handle_sprites(Renderer_Handler* renderer_handler) {
		const Uint8* key_state = SDL_GetKeyboardState(NULL);

		if (key_state[SDL_SCANCODE_G]) {
			this->controller->grab(this->context);
		}

		if (key_state[SDL_SCANCODE_S]) {
			cout << "SCAN_CONTROLLER" << endl;
			this->controller = new Scan_Controller;

			unsigned int n = get_in_number();
			this->autoscan_depth->Change_Text(to_string(n).data());
			this->controller->scan(this->context, renderer_handler, n);
			this->controller = new Manual_Controller;
		}

		if (key_state[SDL_SCANCODE_A]) {
			cout << "AUTO_CONTROLLER" << endl;
			//this->controller = new Auto_Controller;
			Auto_Controller* net = new Auto_Controller();
			//while (net->apples_on_map(this->context)) {
				//std::vector<movement> way = net->find_way_controller(this->context);
				//for (int i = 0; i < way.size(); ++i) {
				//	this->controller->move_collector(this->context, way[i]);
				//	this->controller->grab(this->context);
				//	renderer_handler->Update_Render(this->controller->get_render_map(this->context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
				//	SDL_Delay(100);
				//}

			//}
			
			//this->controller->auto_grab(this->context);
			this->controller = new Manual_Controller;
		}

		if (key_state[SDL_SCANCODE_LCTRL] && key_state[SDL_SCANCODE_B]) {
			this->controller->sapper_on(this->context);
		}

		if (key_state[SDL_SCANCODE_LALT] && key_state[SDL_SCANCODE_B]) {

		}

		scores->Change_Text(to_string(this->context->RC->get_apples()).data());


		controll_robot_collector(key_state);
		if (context->map->robot_collector_exist() == false) {
			death_screen->Show();

			bool quit = false;
			SDL_Event event;
			while (!quit) {
				while (SDL_PollEvent(&event))
				{
					const Uint8* t_key_state = SDL_GetKeyboardState(NULL);
					SDL_PumpEvents();
					if (t_key_state[SDL_SCANCODE_ESCAPE]) {
						quit = true;
						exit(0);
					}

					
				}
				if (death_screen->rect.h < da.top_border) {
					death_screen->rect.x -= da.right_border / 200;
					death_screen->rect.y -= da.top_border / 200;
					death_screen->rect.w += da.right_border / 100;
					death_screen->rect.h += da.top_border / 100;
				}
				renderer_handler->Update_Render(this->controller->get_render_map(this->context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));

			}
		}
		renderer_handler->Update_Render(this->controller->get_render_map(this->context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size));
		//return this->controller->get_render_map(this->context, da.right_border / sprite_size, da.top_border / sprite_size, (da.center_x) / sprite_size + 1, (da.center_y) / sprite_size);
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

	inline unsigned int get_in_number() {
		unsigned int n = 0;

		bool is_return = false;
		SDL_Event Event;

		while (!is_return) {
			while (SDL_PollEvent(&Event)) {
				SDL_PumpEvents();
				const Uint8* t_key_state = SDL_GetKeyboardState(NULL);
				if (t_key_state[SDL_SCANCODE_RETURN] || t_key_state[SDL_SCANCODE_KP_ENTER]) {
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
			if (this->controller->move_collector(this->context, movement::right) == false) {
				return;
			}
			this->controller->scan(this->context);
			background->rect.x -= sprite_size;
			if (robot_collector->flip != SDL_FLIP_NONE) {
				robot_collector->flip = SDL_FLIP_NONE;
			}
		}
		else if (key_state[SDL_SCANCODE_LEFT] && !left) {
			if (this->controller->move_collector(this->context, movement::left) == false) {
				return;
			}
			this->controller->scan(this->context);
			background->rect.x += sprite_size;
			if (robot_collector->flip != SDL_FLIP_HORIZONTAL) {
				robot_collector->flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (key_state[SDL_SCANCODE_UP] && !up) {
			if (this->controller->move_collector(this->context, movement::up) == false) {
				return;
			}
			this->controller->scan(this->context);
			background->rect.y += sprite_size;
		}
		else if (key_state[SDL_SCANCODE_DOWN] && !down) {
			if (this->controller->move_collector(this->context, movement::down) == false) {
				return;
			}
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

	Drawing_Area da(
		wp.w + sprite_size - (wp.w % sprite_size) / 2, // right
		-(wp.w % sprite_size) / 2,						// left
		wp.h + sprite_size - (wp.w % sprite_size) / 2,	// top
		-(wp.h % sprite_size) / 2,						// bot
		(wp.w + sprite_size) / 2 - (((wp.w + sprite_size) / 2) % sprite_size) - (wp.w % sprite_size) / 2, // center x
		(wp.h + sprite_size) / 2 - (((wp.h + sprite_size) / 2) % sprite_size) - (wp.w % sprite_size) / 2); // center y


	g.Create_Renderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, da);

	Sprite* background = new Sprite("background.png", SDL_Rect_(-512 * sprite_size, -512 * sprite_size, 1024 * sprite_size, 1024 * sprite_size), visible);

	Sprite* robot_collector = new Sprite("robot2.PNG", SDL_Rect_(da.center_x, da.center_y, sprite_size, sprite_size), visible);

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

	Text_Sprite* scores = new Text_Sprite("Hello", 24, SDL_Color_(100, 100, 0), SDL_Rect_(da.center_x, 0, 100, 100));
	Text_Sprite* autoscan_depth = new Text_Sprite("", 24, SDL_Color_(0, 0, 0), SDL_Rect_(da.right_border - 400, 0, 200, 100));


	Sprite* GUI = new Sprite("GUI.png", SDL_Rect_(0, 0, wp.w, wp.h));
	GUI->Show();

	Sprite* death_screen = new Sprite("death_screen.png", SDL_Rect_(da.center_x-1, da.center_y-1, 2, 2), invisible);


	vector<Sprite*> other_sprites = { GUI, scores, autoscan_depth, death_screen};

	Game::Sprite_Controller* main_controller = new Main_Controller(robot_collector, background, da, controller, context, scores, autoscan_depth, death_screen);
	g.Set_Sprites(robot_collector, apple, rock, unknown, bomb, background, other_sprites);
	g.mainloop(main_controller);

	TTF_Quit();
	delete main_controller;
	delete robot_collector, apple, bomb, rock, unknown, controller, context, scores, autoscan_depth, GUI;

	return 0;
}
