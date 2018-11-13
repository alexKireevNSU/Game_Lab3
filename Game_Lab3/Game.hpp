#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <exception>
#include <vector>

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

	struct SDL_Rect_ : public SDL_Rect {
		SDL_Rect_(int x, int y, int w, int h);
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

		Sprite(const char* path, SDL_Rect_ rect, Sprite_State sprite_state) noexcept;

		void Hide();
		void Fog();
		void Show();

		void Load_Texture(SDL_Renderer* renderer);

		~Sprite();
	};

	enum block { rock, empty, apple, bomd };
	enum movement { right, left, up, down };

	class God {
	private:
		Playground main_map;
		size_t coll_x, coll_y;
	public:
		God();
		God(Playground & pg);
		void Create_Robot_Collector(size_t x, size_t y);
		std::vector<block> get_robot_area();
		void Move_Robot_Collector(movement m);
	};

	class Robot : public Sprite {
	public:
		Robot();
		virtual ~Robot() = 0;
	};



	class Robot_Collector : public Robot{
	private:
		Playground map;
		size_t my_x, my_y;
		//class Scan {
		//	Scan();
		//	void doing(size_t N);
		//};
		//class Move {
		//	Move();
		//	void doing(movement m);
		//};
		//class Grab {
		//	Grab();
		//	void doing();
		//};
	public:
		Robot_Collector(size_t my_x, size_t my_y);
		void move(movement m);
		void scan(size_t N);
		void Grab();
		virtual ~Robot_Collector();
	};

	class Robot_Sapper : public Robot{
	private:
		Playground map;
		size_t my_x, my_y;

		class Demine {
			Demine();
			~Demine();
			void doing();
		};

	public:
		Robot_Sapper();
		Robot_Sapper(Robot_Collector & rc);
		Demine demine;

		virtual ~Robot_Sapper();
	};

	class Playground {
	private:
		block* map;
		size_t length, width;

	public:
		Playground();
		Playground(FILE* fin);
		Playground(Playground & pg);
		block get_data(size_t, size_t);

		virtual ~Playground();
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
		std::vector<Sprite*> sprites;
		Sprite* player;
		Sprite* background;
	public:
		Renderer_Handler(SDL_Window* window, int driver_index, Uint32 renderer_flags);
		~Renderer_Handler();
		void Set_Sprites(Sprite* player, Sprite* background, std::vector<Sprite*> sprites);
		void Add_Sprites(Sprite* sprite);
		void Update_Render();
	};

	class Sprite_Controller {
	public:
		virtual void handle_sprites();
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

		void Create_Renderer(int driver_index, Uint32 renderer_flags);

		void Set_Sprites(Sprite* player, Sprite* background, std::vector<Sprite*> sprites) noexcept;

		void Add_Sprite(Sprite* sprite);

		void mainloop(Sprite_Controller* sprite_controller);
	};

}