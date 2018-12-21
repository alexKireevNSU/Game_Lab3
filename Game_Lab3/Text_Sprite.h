#pragma once
#include "Game.h"
using namespace Game;

struct Text_Sprite : public Sprite {
	std::string text;
	Text_Sprite(const char* text, SDL_Rect_ rect, Sprite_State sprite_state = visible);
	virtual void Load_Texture(SDL_Renderer* renderer);

	virtual ~Text_Sprite();
};