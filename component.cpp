#pragma once
#include "component.h"
#include <raylib.h>
#include "consts.h"
#include "sprite.h"
#include "asset_manager.h"



void GraphicsComponent::Draw()
{
	SpriteSheet* ss = (SpriteSheet*)GetAssetManager()->GetAsset(sprite_id);

	Rectangle dest = { pos.x, pos.y, size.x, size.y };
	Rectangle frame = ss->GetFrame(frame_pos, flip_h, flip_v);

	if (ss)
	{
		DrawTexturePro(ss->texture, frame, dest, { origin.x, origin.y }, 0, WHITE);
	}
}


void GraphicsComponent::SetFlipOrigin()
{
	if (flip_h)
	{
		origin.x = size.x / 2;
	}
	else
	{
		origin.x = 0;
	}
}


void GraphicsComponent::NextSprite()
{
	if ((GetTic() % tic) == 0)
	{
		frame_pos.x++;
	}
}
