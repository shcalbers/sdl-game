#include "SDL_Game/Client/Visuals.hpp"

Sprite::Sprite() : sheetTexture(nullptr), Rectangle( {0, 0, 0, 0} ) {}

Sprite::Sprite(Texture * texture, const Rectangle & bounds)
	: sheetTexture(texture), Rectangle(bounds)
{}

void Sprite::Draw(Renderer & renderer, const Rectangle * dstrect)
{
	renderer.DrawTexture(*sheetTexture, this, dstrect);

	return;
}

Sprite::~Sprite() {}
