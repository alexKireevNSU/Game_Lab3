#pragma once
#include "Game.h"
#include <SDL_ttf.h>
using namespace Game;

struct SDL_Color_ : public SDL_Color {
	int R;
	int G;
	int B;

	SDL_Color_(int R, int G, int B);
};

struct Text_Sprite : public Sprite {
	SDL_Renderer* renderer;
	std::string text;
	TTF_Font* font;
	SDL_Surface * surface;
	SDL_Color color;

	unsigned int font_size;
	Text_Sprite(const char* text, unsigned int font_size, SDL_Color color, SDL_Rect_ rect, Sprite_State sprite_state = visible);
	virtual void Load_Texture(SDL_Renderer* renderer);
	void Change_Text(const char * text);

	virtual ~Text_Sprite();
};