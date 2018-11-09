#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <exception>
#include <vector>

class Game_Exception : public std::exception {
	std::string whatStr;
public:
	Game_Exception(std::string &&whatStr) noexcept : whatStr(std::move(whatStr)) { }
	Game_Exception(const std::string &whatStr) noexcept : whatStr(whatStr) { }

	const char* what() const noexcept override {
		return whatStr.c_str();
	}

	~Game_Exception() noexcept = default;
};

struct SDL_Rect_ : public SDL_Rect {
	SDL_Rect_(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};

enum Sprite_State {
	invisible, fog, visible
};

struct Sprite {
	const char* path;
	SDL_Rect rect;
	Sprite_State sprite_state;

	int angle = 0;
	SDL_Point point;

	SDL_RendererFlip flip;

	SDL_Texture* texture;

	Sprite(const char* path, SDL_Rect_ rect, Sprite_State sprite_state) noexcept {
		this->path = path;
		this->rect = rect;
		this->sprite_state = sprite_state;
		point.x = rect.x / 2;
		point.y = rect.y / 2;

		flip = SDL_FLIP_NONE;
	}

	void Hide_Sprite() {
		sprite_state = invisible;
	}
	void Fog_Sprite() {
		sprite_state = fog;
	}
	void Show_Sprite() {
		sprite_state = visible;
	}

	void Load_Texture(SDL_Renderer* renderer) {
		texture = IMG_LoadTexture(renderer, path);
		if (texture == nullptr) {
			throw Game_Exception(IMG_GetError());
		}
	}

	~Sprite()
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
};

struct Window_Properties {
	const char* title;
	int x;
	int y;
	int w;
	int h;
	Uint32 flags;
	Window_Properties(const char* title, int x, int y, int w, int h, Uint32 flags) {
		this->title = title;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->flags = flags;
	}
	Window_Properties() {}
};

class Window_Handler {
	SDL_Window* window;
public:
	Window_Handler(SDL_DisplayMode dm, Window_Properties wp) {
		window = SDL_CreateWindow(wp.title, wp.x, wp.y, wp.w, wp.h, wp.flags);
		if (window == nullptr) throw Game_Exception("SDL_CreateWindow error");

	}
	~Window_Handler()
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_Window* Get_Window() {
		return window;
	}
};

class Renderer_Handler {
	SDL_Renderer* renderer;
	std::vector<Sprite*> sprites;
	Sprite* player;
	Sprite* background;
public:
	Renderer_Handler(SDL_Window* window, int driver_index, Uint32 renderer_flags) {
		renderer = SDL_CreateRenderer(window, driver_index, renderer_flags);
		if (renderer == nullptr) throw Game_Exception("SDL_CreateRenderer error");
	}
	~Renderer_Handler()
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	void Set_Sprites(Sprite* player, Sprite* background, std::vector<Sprite*> sprites) {
		this->player = player;
		this->background = background;
		this->sprites = sprites;

		this->player->Load_Texture(renderer);
		this->background->Load_Texture(renderer);
		for (size_t i = 0; i < sprites.size(); ++i) {
			this->sprites[i]->Load_Texture(renderer);
		}
	}

	void Add_Sprites(Sprite* sprite) {
		sprites.push_back(sprite);
	}

	void Update_Render() {
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

};

class Game {
	Window_Handler* window_handler = nullptr;
	Renderer_Handler* renderer_handler = nullptr;

	SDL_DisplayMode display_mode;
	Window_Properties window_properties;

public:
	Game() {
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

	~Game() {
		delete window_handler;
		delete renderer_handler;
		SDL_Quit();
	}

	void Create_Window(Window_Properties window_properties) {
		this->window_properties = window_properties;
		this->window_handler = new Window_Handler(display_mode, window_properties);
	}

	void Create_Renderer(int driver_index, Uint32 renderer_flags) {
		if (window_handler == nullptr) {
			throw Game_Exception("Render_Handler_init error: no window_handler");
		}
		renderer_handler = new Renderer_Handler(window_handler->Get_Window(), driver_index, renderer_flags); //TODO
	}

	void Set_Sprites(Sprite* player, Sprite* background, std::vector<Sprite*> sprites) noexcept {
		renderer_handler->Set_Sprites(player, background, (sprites));
	}

	void Add_Sprite(Sprite* sprite) {
		renderer_handler->Add_Sprites(sprite);
	}

	void mainloop() {
		if (renderer_handler == nullptr)
			throw Game_Exception("mainloop error: no renderer_handler");
		bool quit = false;
		SDL_Event event;
		while (!quit) {
			while (SDL_PollEvent(&event))
			{
				SDL_PumpEvents();
				if (event.type == SDL_QUIT) quit = true;
			}


			renderer_handler->Update_Render();
		}
	}
};


int main(int argc, char** argv) {

	Window_Properties wp("jopa", 50, 50, 1000, 800, SDL_WINDOW_SHOWN);

	Game g;

	g.Create_Window(wp);
	g.Create_Renderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Sprite* player = new Sprite("C:/Users/goodaman/Pictures/1.png", SDL_Rect_(0, 0, 200, 200), visible);

	Sprite* background = new Sprite("C:/Users/goodaman/Pictures/3.jpg", SDL_Rect_(0, 0, 1000, 800), visible);

	std::vector<Sprite*> sprites;
	for (size_t i = 0; i < 10; ++i) {
		for (size_t j = 0; j < 10; ++j) {
			sprites.push_back(new Sprite("C:/Users/goodaman/Pictures/bl.jpg", SDL_Rect_(j * 50, i * 50, 50, 50), visible));
		}
	}

	g.Set_Sprites(player, background, sprites);

	g.mainloop();


	return 0;
}