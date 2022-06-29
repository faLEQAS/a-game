#include "sprite.h"



Rectangle SpriteSheet::GetFrame(Vector2D pos, bool flip_h, bool flip_v)
{
	Vector2D sprite_size = {};
	sprite_size.x = texture.width / size.x;
	sprite_size.y = texture.height / size.y;

	Rectangle frame = {};
	frame.x = sprite_size.x * pos.x;
	frame.y = sprite_size.y * pos.y;
	frame.width = sprite_size.x;
	frame.height = sprite_size.y;

	if (flip_h)
	{
		frame.width *= -1;
	}
	if (flip_v)
	{
		frame.height *= -1;
	}

	return frame;
}