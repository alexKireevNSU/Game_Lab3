#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include "enums.h"

namespace Game {



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

	struct SDL_Rect_ : public SDL_Rect {
		SDL_Rect_(int x, int y, int w, int h);
	};

	enum Sprite_State {
		invisible, visible
	};

	struct Sprite {
		const char* path;
		SDL_Rect rect;
		Sprite_State sprite_state;

		int angle = 0;
		SDL_Point point;

		SDL_RendererFlip flip;

		SDL_Texture* texture;

		Sprite() {};
		Sprite(SDL_Rect_ rect, Sprite_State sprite_state = visible) noexcept;
		Sprite(const char* path, SDL_Rect_ rect, Sprite_State sprite_state = visible) noexcept;

		void Hide();
		void Show();

		virtual void Load_Texture(SDL_Renderer* renderer);

		virtual ~Sprite();
	};

	struct Window_Properties {
		const char* title;
		int x;
		int y;
		int w;
		int h;
		Uint32 flags;
		Window_Properties(const char* title, int x, int y,
			int w, int h, Uint32 flags) :
			title(title), x(x), y(y), w(w), h(h), flags(flags) {}
		Window_Properties() {}
	};

	class Window_Handler {
		SDL_Window* window;
	public:
		Window_Handler(SDL_DisplayMode dm, Window_Properties wp);
		~Window_Handler();
		SDL_Window* Get_Window();
	};

	class Renderer_Handler {
		SDL_Renderer* renderer;
		Sprite* player;
		Sprite* robot_sapper;
		Sprite* apple;
		Sprite* rock;
		Sprite* unknown;
		Sprite* bomb;
		Sprite* background;
		std::vector<Sprite*> other_sprites;

		
	public:
		Drawing_Area da;
		Renderer_Handler(SDL_Window* window, int driver_index, Uint32 renderer_flags, Drawing_Area da);
		~Renderer_Handler();
		void Set_Sprites(Sprite* player, Sprite* robot_sapper, Sprite* apple, Sprite* rock, Sprite* unknown, Sprite* bomb, Sprite* background, std::vector<Sprite*> other_sprites);
		void Add_Sprites(Sprite* sprite);
		void Update_Render(std::vector<std::vector<block>> render_map);
		void move_background(movement m);
	};

	class Sprite_Controller {
	public:
		virtual void handle_sprites(Renderer_Handler*);
		virtual ~Sprite_Controller();
	};

	class Game_Handler {
		Window_Handler* window_handler = nullptr;
		Renderer_Handler* renderer_handler = nullptr;

		SDL_DisplayMode display_mode;
		Window_Properties window_properties;

	public:
		Game_Handler();

		~Game_Handler();

		void Create_Window(Window_Properties window_properties);

		void Create_Renderer(int driver_index, Uint32 renderer_flags, Drawing_Area da);

		void Set_Sprites(Sprite* player, Sprite* robot_sapper, Sprite* apple, Sprite* rock, Sprite* unknown, Sprite* bomb, Sprite* background, std::vector<Sprite*> other_sprites) noexcept;

		void Add_Sprite(Sprite* sprite);

		void mainloop(Sprite_Controller* sprite_controller);
	};

}