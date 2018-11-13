#include "Game.hpp"
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

Sprite::Sprite(const char* path, SDL_Rect_ rect, Sprite_State sprite_state) noexcept {
	this->path = path;
	this->rect = rect;
	this->sprite_state = sprite_state;
	point.x = rect.x / 2;
	point.y = rect.y / 2;

	flip = SDL_FLIP_NONE;
}
void Sprite::Hide() {
	sprite_state = invisible;
}
void Sprite::Fog() {
	sprite_state = fog;
}
void Sprite::Show() {
	sprite_state = visible;
}
void Sprite::Load_Texture(SDL_Renderer* renderer) {
	texture = IMG_LoadTexture(renderer, path);

	
	if (texture == nullptr) {
		throw Game_Exception(IMG_GetError());
	}
}
Sprite::~Sprite()
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

Renderer_Handler::Renderer_Handler(SDL_Window* window, int driver_index, Uint32 renderer_flags) {
	renderer = SDL_CreateRenderer(window, driver_index, renderer_flags);
	if (renderer == nullptr) throw Game_Exception("SDL_CreateRenderer error");
}
Renderer_Handler::~Renderer_Handler()
{
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
}
void Renderer_Handler::Set_Sprites(Sprite* player, Sprite* background, std::vector<Sprite*> sprites) {
	this->player = player;
	this->background = background;
	this->sprites = sprites;
	this->player->Load_Texture(renderer);
	this->background->Load_Texture(renderer);
	for (size_t i = 0; i < sprites.size(); ++i) {
		this->sprites[i]->Load_Texture(renderer);
	}

}
void Renderer_Handler::Add_Sprites(Sprite* sprite) {
	sprites.push_back(sprite);
}
void Renderer_Handler::Update_Render() {
	SDL_RenderClear(renderer);

	if (background != nullptr)
		SDL_RenderCopyEx(renderer, background->texture, NULL,
			&background->rect, background->angle, &background->point, background->flip);

	for (size_t i = 0; i < sprites.size(); ++i) {
		if (sprites[i] != nullptr)
			SDL_RenderCopyEx(renderer, sprites[i]->texture, NULL,
				&sprites[i]->rect, sprites[i]->angle, &sprites[i]->point, sprites[i]->flip);
	}
	if (player != nullptr)
		SDL_RenderCopyEx(renderer, player->texture, NULL,
			&player->rect, player->angle, &player->point, player->flip);

	SDL_RenderPresent(renderer);
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
void Game_Handler::Create_Renderer(int driver_index, Uint32 renderer_flags) {
	if (window_handler == nullptr) {
		throw Game_Exception("Render_Handler_init error: no window_handler");
	}
	renderer_handler = new Renderer_Handler(window_handler->Get_Window(), driver_index, renderer_flags); //TODO
}
void Game_Handler::Set_Sprites(Sprite* player, Sprite* background, std::vector<Sprite*> sprites) noexcept {
	renderer_handler->Set_Sprites(player, background, (sprites));
}
void Game_Handler::Add_Sprite(Sprite* sprite) {
	renderer_handler->Add_Sprites(sprite);
}
void Game_Handler::mainloop(Sprite_Controller* sprite_controller) {
	if (renderer_handler == nullptr)
		throw Game_Exception("mainloop error: no renderer_handler");
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event))
		{
			SDL_PumpEvents();
			if (event.type == SDL_QUIT) quit = true;

			sprite_controller->handle_sprites();
			//_sleep(1000);
		}
		renderer_handler->Update_Render();
	}
}

//----------------------------------------------------------------------------
//----------------------Sprite_Controller methods-----------------------------
//----------------------------------------------------------------------------

void Sprite_Controller::handle_sprites(){}
Sprite_Controller::~Sprite_Controller() {}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------Robot-------------------------------------------
void Robot_Collector::move(movement m) {
	switch (m) {
		case right: {
			block b = this->map.get_data(this->my_x + 1, this->my_y);
			if (b == rock) {
				return;
			}
			++this->my_x;
		}
		case left: {
			block b = this->map.get_data(this->my_x - 1, this->my_y);
			if (b == rock) {
				return;
			}
			--this->my_x;
		}
		case down: {
			block b = this->map.get_data(this->my_x, this->my_y - 1);
			if (b == rock) {
				return;
			}
			--this->my_y;
		}
		case up: {
			block b = this->map.get_data(this->my_x, this->my_y + 1);
			if (b == rock) {
				return;
			}
			++this->my_y;
		}
		default: { return; }
	}
}

void Robot_Collector::Grab() {

}

