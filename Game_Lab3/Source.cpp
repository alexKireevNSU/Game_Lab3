#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <exception>
#include <vector>

#include "Game.h"

using namespace Game;
using namespace std;

class Sp : public Game::Sprite_Controller {
	Sprite* player;
	Uint8 right = 0;
	Uint8 left = 0;
	Uint8 up = 0;
	Uint8 down = 0;
public:
	Sp(Sprite* player) {
		this->player = player;
	}

	void handle_sprites() {
		const Uint8* key_state = SDL_GetKeyboardState(NULL);
		if (key_state[SDL_SCANCODE_RIGHT] && !right) {
			player->rect.x += player->rect.w;
			if (player->flip != SDL_FLIP_NONE) {
				player->flip = SDL_FLIP_NONE;
			}
		}
		else if (key_state[SDL_SCANCODE_LEFT] && !left) {
			player->rect.x -= player->rect.w;
			if (player->flip != SDL_FLIP_HORIZONTAL) {
				player->flip = SDL_FLIP_HORIZONTAL;
			}
		}
		if (key_state[SDL_SCANCODE_UP] && !up) {
			player->rect.y -= player->rect.h;
		}
		else if (key_state[SDL_SCANCODE_DOWN] && !down) {
			player->rect.y += player->rect.h;
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

	Window_Properties wp("LOL", 50, 50, 1000, 800, SDL_WINDOW_SHOWN);
	Game_Handler g;
	g.Create_Window(wp);
	g.Create_Renderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	Sprite* player = new Sprite("janitor.PNG", SDL_Rect_(0, 0, 200, 200), visible);
	Sprite* background = new Sprite("janitor.PNG", SDL_Rect_(0, 0, 1000, 800), visible);
	std::vector<Sprite*> sprites;
	unsigned int size = 75;
	for (size_t i = 0; i < 10; ++i) {
		for (size_t j = 0; j < 10; ++j) {
			sprites.push_back(new Sprite("janitor.PNG", SDL_Rect_(j * size, i * size, size, size), visible));
		}
	}

	g.Set_Sprites(player, background, sprites);
	Game::Sprite_Controller* sc = new Sp(player);
	g.mainloop(sc);

	delete sc;


	return 0;
}