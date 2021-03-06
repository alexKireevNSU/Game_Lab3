#include "Game.h"
#include "Robot.h"
#include <vector>
#include <string>
using namespace Game;
//----------------------------------------------------------------------------
//----------------------SDL_RECT_ methods-------------------------------------
//----------------------------------------------------------------------------

SDL_Rect_::SDL_Rect_(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

//----------------------------------------------------------------------------
//----------------------Sprite methods----------------------------------------
//----------------------------------------------------------------------------

Game::Sprite::Sprite(SDL_Rect_ rect, Sprite_State sprite_state) noexcept {
	this->rect = rect;
	this->sprite_state = sprite_state;
	point.x = rect.x / 2;
	point.y = rect.y / 2;

	flip = SDL_FLIP_NONE;
}
Game::Sprite::Sprite(const char* path, SDL_Rect_ rect, Sprite_State sprite_statee) noexcept{
	this->path = path;
	this->rect = rect;
	this->sprite_state = sprite_state;
	point.x = rect.x / 2;
	point.y = rect.y / 2;

	flip = SDL_FLIP_NONE;
}

void Game::Sprite::Hide() {
	sprite_state = invisible;
}
void Game::Sprite::Show() {
	sprite_state = visible;
}
void Game::Sprite::Load_Texture(SDL_Renderer* renderer) {
	try {
		texture = IMG_LoadTexture(renderer, path);
	}
	catch (...) {
		throw Game_Exception(IMG_GetError());
	}
	if (texture == nullptr) {
		throw Game_Exception(IMG_GetError());
	}
}
Game::Sprite::~Sprite()
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
}
//----------------------------------------------------------------------------
//----------------------Window_Handler methods--------------------------------
//----------------------------------------------------------------------------

Window_Handler::Window_Handler(SDL_DisplayMode dm, Window_Properties wp) {
	window = SDL_CreateWindow(wp.title, wp.x, wp.y, wp.w, wp.h, wp.flags);
	if (window == nullptr) throw Game_Exception("SDL_CreateWindow error");
}
Window_Handler::~Window_Handler()
{
	SDL_DestroyWindow(window);
	window = nullptr;
}
SDL_Window* Window_Handler::Get_Window() {
	return window;
}

//----------------------------------------------------------------------------
//----------------------Renderer_Handler methods------------------------------
//----------------------------------------------------------------------------

Renderer_Handler::Renderer_Handler(SDL_Window* window, int driver_index, Uint32 renderer_flags, Drawing_Area da) {
	renderer = SDL_CreateRenderer(window, driver_index, renderer_flags);
	if (renderer == nullptr) throw Game_Exception("SDL_CreateRenderer error");
	this->da = da;
}
Renderer_Handler::~Renderer_Handler()
{
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
}
void Renderer_Handler::Set_Sprites(Sprite* player, Sprite* robot_sapper, Sprite* apple, Sprite* rock, Sprite* unknown, Sprite* bomb, Sprite* background, std::vector<Sprite*> other_sprites) {
	this->player = player;
	this->robot_sapper = robot_sapper;
	this->apple = apple;
	this->rock = rock;
	this->unknown = unknown;
	this->bomb = bomb;
	this->background = background;
	this->other_sprites = other_sprites;

	this->player->Load_Texture(renderer);
	this->robot_sapper->Load_Texture(renderer);
	this->apple->Load_Texture(renderer);
	this->rock->Load_Texture(renderer);
	this->unknown->Load_Texture(renderer);
	this->bomb->Load_Texture(renderer);
	this->background->Load_Texture(renderer);
	
	for (unsigned int i = 0; i < other_sprites.size(); ++i) {
		this->other_sprites[i]->Load_Texture(renderer);
	}

}
void Renderer_Handler::Add_Sprites(Sprite* sprite) {
	//sprites.push_back(sprite);
}
void Renderer_Handler::Update_Render(std::vector<std::vector<block>> render_map) {
	SDL_RenderClear(renderer);

	if (background != nullptr)
		SDL_RenderCopyEx(renderer, background->texture, NULL,
			&background->rect, background->angle, &background->point, background->flip);

	for (size_t x = 0; x < render_map.size(); ++x) {
		for (size_t y = 0; y < render_map[x].size(); ++y) {
			if (render_map[x][y] == block::rock) {
				SDL_Rect rect(rock->rect);
				rect.x += sprite_size * x;
				rect.y += sprite_size * y;
				SDL_RenderCopyEx(renderer, rock->texture, NULL,
					&rect, rock->angle, &rock->point, rock->flip);
			}
			if (render_map[x][y] == block::apple) {
				SDL_Rect rect(apple->rect);
				rect.x += sprite_size * x;
				rect.y += sprite_size * y;
				SDL_RenderCopyEx(renderer, apple->texture, NULL,
					&rect, apple->angle, &apple->point, apple->flip);
			}
			if (render_map[x][y] == block::unknown) {
				SDL_Rect rect(unknown->rect);
				rect.x += sprite_size * x;
				rect.y += sprite_size * y;
				SDL_RenderCopyEx(renderer, unknown->texture, NULL,
					&rect, unknown->angle, &unknown->point, unknown->flip);
			}
			if (render_map[x][y] == block::bomb) {
				SDL_Rect rect(bomb->rect);
				rect.x += sprite_size * x;
				rect.y += sprite_size * y;
				SDL_RenderCopyEx(renderer, bomb->texture, NULL,
					&rect, bomb->angle, &bomb->point, bomb->flip);
			}
			if (render_map[x][y] == block::robot_sapper) {
				SDL_Rect rect(robot_sapper->rect);
				rect.x += sprite_size * x;
				rect.y += sprite_size * y;
				SDL_RenderCopyEx(renderer, robot_sapper->texture, NULL,
					&rect, robot_sapper->angle, &robot_sapper->point, robot_sapper->flip);
			}
		}
	}

	if (player != nullptr)
		SDL_RenderCopyEx(renderer, player->texture, NULL,
			&player->rect, player->angle, &player->point, player->flip);
	
	for (unsigned int i = 0; i < other_sprites.size(); ++i) {
		if (other_sprites[i] != nullptr && (other_sprites[i]->sprite_state == visible)) {
			SDL_RenderCopyEx(renderer, other_sprites[i]->texture, NULL,
				&other_sprites[i]->rect, other_sprites[i]->angle, &other_sprites[i]->point, other_sprites[i]->flip);
		}
	}
	
		
	SDL_RenderPresent(renderer);
}

void Renderer_Handler::move_background(movement m) {
	switch (m) {
	case movement::up:	{	this->background->rect.y += sprite_size; return; }
	case movement::right: {	this->background->rect.x -= sprite_size; return; }
	case movement::down: {	this->background->rect.y -= sprite_size; return; }
	case movement::left: {	this->background->rect.x += sprite_size; return; }
	default: { return; }
	}
}

//----------------------------------------------------------------------------
//----------------------Game_Handler methods----------------------------------
//----------------------------------------------------------------------------

Game_Handler::Game_Handler() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL_Init_error: " << SDL_GetError() << std::endl;
		throw Game_Exception("SDL_Init_error");
	}
	int req = SDL_GetDesktopDisplayMode(0, &display_mode);
	if (req != 0) {
		std::cout << "SDL_Init_error: " << SDL_GetError() << std::endl;
		throw Game_Exception("SDL_Init_error");
	}
}
Game_Handler::~Game_Handler() {
	delete window_handler;
	delete renderer_handler;
	SDL_Quit();
}
void Game_Handler::Create_Window(Window_Properties window_properties) {
	this->window_properties = window_properties;
	this->window_handler = new Window_Handler(display_mode, window_properties);
}
void Game_Handler::Create_Renderer(int driver_index, Uint32 renderer_flags, Drawing_Area da) {
	if (window_handler == nullptr) {
		throw Game_Exception("Render_Handler_init error: no window_handler");
	}
	renderer_handler = new Renderer_Handler(window_handler->Get_Window(), driver_index, renderer_flags, da); //TODO
}
void Game_Handler::Set_Sprites(Sprite* player, Sprite* robot_sapper, Sprite* apple, Sprite* rock, Sprite* unknown, Sprite* bomb, Sprite* background, std::vector<Sprite*> other_sprites) noexcept {
	renderer_handler->Set_Sprites(player, robot_sapper, apple, rock, unknown, bomb, background, other_sprites);
}
void Game_Handler::Add_Sprite(Sprite* sprite) {
	renderer_handler->Add_Sprites(sprite);
}
void Game_Handler::mainloop(Sprite_Controller* sprite_controller) {
	if (renderer_handler == nullptr)
		throw Game_Exception("mainloop error: no renderer_handler");
	bool quit = false;
	SDL_Event event;

	//std::vector<std::vector<block>> render_map;
	while (!quit) {
		while (SDL_PollEvent(&event))
		{
			SDL_PumpEvents();
			if (event.type == SDL_QUIT) quit = true;
			sprite_controller->handle_sprites(renderer_handler);
		}
	}
}

//----------------------------------------------------------------------------
//----------------------Sprite_Controller methods-----------------------------
//----------------------------------------------------------------------------

void Sprite_Controller::handle_sprites(Renderer_Handler*) { }
Sprite_Controller::~Sprite_Controller() {}