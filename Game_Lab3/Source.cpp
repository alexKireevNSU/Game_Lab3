#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <exception>
#include <vector>

#include "Game.hpp"

using namespace Game;

class Sp : public Game::Sprite_Controller {
public:
	void handle_sprites() {
		return;
	}
};

int main(int argc, char** argv) {

	Window_Properties wp("LOL", 50, 50, 1000, 800, SDL_WINDOW_SHOWN);

	Game_Handler g;

	g.Create_Window(wp);
	g.Create_Renderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Sprite* player = new Sprite("C:/Users/goodaman/Pictures/1.png", SDL_Rect_(0, 0, 200, 200), visible);

	Sprite* background = new Sprite("C:/Users/goodaman/Pictures/3.jpg", SDL_Rect_(0, 0, 1000, 800), visible);

	std::vector<Sprite*> sprites;
	unsigned int size = 75;
	for (size_t i = 0; i < 10; ++i) {
		for (size_t j = 0; j < 10; ++j) {
			sprites.push_back(new Sprite("C:/Users/goodaman/Pictures/bl.jpg", SDL_Rect_(j * size, i * size, size, size), visible));
		}
	}

	g.Set_Sprites(player, background, sprites);

	Game::Sprite_Controller* sc = new Sp();

	g.mainloop(sc);

	delete sc;


	return 0;
}