#include "Text_Sprite.h"
#include <SDL_ttf.h>
#include "Game.h"
//----------------------------------------------------------------------------
//----------------------Text_Sprite methods----------------------------------------
//----------------------------------------------------------------------------

Text_Sprite::Text_Sprite(const char* text, SDL_Rect_ rect, Sprite_State sprite_state) : Sprite(rect, sprite_state)
{
	this->text = *(new std::string(text));
}
void Text_Sprite::Load_Texture(SDL_Renderer * renderer)
{

}
Text_Sprite::~Text_Sprite() {}