#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <exception>
#include <vector>
#include <map>

#include "Game.h"
#include "Robot.h"

using namespace Game;
using namespace std;
using namespace Robots;

struct Drawing_Area {
	int right_border;
	int left_border;
	int top_border;
	int bot_border;
	Drawing_Area(
		int right_border,
		int left_border,
		int top_border,
		int bot_border) :
		right_border(right_border),
		left_border(left_border),
		top_border(top_border),
		bot_border(bot_border) {}
	Drawing_Area() {}
};



class Main_Controller: public Game::Sprite_Controller {
	Sprite* robot_collector;
	Drawing_Area da;
	Uint8 right = 0;
	Uint8 left = 0;
	Uint8 up = 0;
	Uint8 down = 0;
public:
	Main_Controller(Sprite* robot_collector, Drawing_Area da) {
		this->robot_collector = robot_collector;
		this->da = da;
	}

	void handle_sprites() {
		const Uint8* key_state = SDL_GetKeyboardState(NULL);
		controll_robot_collector(key_state);
		check_drawing_area_out();
	}

	void check_drawing_area_out() {
		if (robot_collector->rect.x + robot_collector->rect.w > da.right_border) {
			robot_collector->rect.x = da.left_border;
		}
		if (robot_collector->rect.x < da.left_border) {
			robot_collector->rect.x = da.right_border - robot_collector->rect.w;
		}
		if (robot_collector->rect.y + robot_collector->rect.h > da.top_border) {
			robot_collector->rect.y = da.bot_border;
		}
		if (robot_collector->rect.y < da.bot_border) {
			robot_collector->rect.y = da.top_border - robot_collector->rect.h;
		}
	}

	void controll_robot_collector(const Uint8* key_state) {
		if (key_state[SDL_SCANCODE_RIGHT] && !right) {
			//robot_collector->rect.x += robot_collector->rect.w;
			if (robot_collector->flip != SDL_FLIP_NONE) {
				robot_collector->flip = SDL_FLIP_NONE;
			}
		}
		else if (key_state[SDL_SCANCODE_LEFT] && !left) {
			//robot_collector->rect.x -= robot_collector->rect.w;
			if (robot_collector->flip != SDL_FLIP_HORIZONTAL) {
				robot_collector->flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (key_state[SDL_SCANCODE_UP] && !up) {
			//robot_collector->rect.y -= robot_collector->rect.h;
		}
		else if (key_state[SDL_SCANCODE_DOWN] && !down) {
			//robot_collector->rect.y += robot_collector->rect.h;
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

	Window_Properties wp("LOL", 50, 50, display_mode.w, display_mode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	//Window_Properties wp("LOL", 50, 50, 1000, 800, SDL_WINDOW_SHOWN);

	g.Create_Window(wp);
	g.Create_Renderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Sprite* background = new Sprite("background.jpg", SDL_Rect_(0, 0, 1000 * 75, 1000 * 75), visible);

	Drawing_Area da(
		wp.w - (wp.w % 75) / 2,
		(wp.w % 75) / 2,
		wp.h - (wp.h % 75) / 2,
		(wp.h % 75) / 2);

	//std::vector<Sprite*> sprites;
	/*int size = 75;
	for (int i = da.bot_border; i < da.top_border; i += size) {
		for (int j = da.left_border; j < da.right_border; j += size) {
			sprites.push_back(new Sprite("jd.jpg", SDL_Rect_(j, i, size, size), visible));
		}
	}*/
	Robot_Collector* robot_collector = new Robot_Collector("janitor.PNG", SDL_Rect_(da.left_border, da.top_border, 75, 75), visible);

	Sprite* apple;
	Sprite* rock;
	Sprite* unknown;

	g.Set_Sprites(robot_collector, apple, rock, unknown, background);

	Game::Sprite_Controller* main_controller = new Main_Controller(robot_collector, da);
	g.mainloop(main_controller);

	delete main_controller;


	return 0;
}
