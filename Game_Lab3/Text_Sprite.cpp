#include "Text_Sprite.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Game.h"

SDL_Color_::SDL_Color_(int R, int G, int B) {
	this->R = R;
	this->G = G;
	this->B = B;
}
//----------------------------------------------------------------------------
//----------------------Text_Sprite methods-----------------------------------
//----------------------------------------------------------------------------

Text_Sprite::Text_Sprite(const char* text, unsigned int font_size, SDL_Color color, SDL_Rect_ rect, Sprite_State sprite_state) : Sprite(rect, sprite_state)
{
	this->font_size = font_size;
	this->color = color;
	this->text = *(new std::string(text));
}
void Text_Sprite::Load_Texture(SDL_Renderer * renderer)
{
	this->renderer = renderer;
	this->font = TTF_OpenFont("arial.ttf", this->font_size); // добавить try catch
	this->surface = TTF_RenderText_Solid(this->font, this->text.data(), color);
	this->texture = SDL_CreateTextureFromSurface(renderer, surface);

}
void Text_Sprite::Change_Text(const char* text) {
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	this->text.clear();
	this->text = text;
	this->font = TTF_OpenFont("arial.ttf", this->font_size); // добавить try catch
	this->surface = TTF_RenderText_Solid(this->font, this->text.data(), color);
	this->texture = SDL_CreateTextureFromSurface(renderer, surface);

}
Text_Sprite::~Text_Sprite() {
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}