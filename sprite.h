#pragma once
#include <raylib.h>
#include "consts.h"
#include "asset.h"


enum
{
	SPRITE_NONE,
	SPRITE_PUNK_IDLE,
	SPRITE_PUNK_WALK,
	SPRITE_PUNK_KICK,
	SPRITE_TILE
};


struct Sprite : public Asset
{
	Texture2D texture;
};


struct SpriteSheet : public Asset
{
	Texture2D texture;
	Vector2D size = {};

	Rectangle GetFrame(Vector2D pos, bool flip_h, bool flip_y);
};