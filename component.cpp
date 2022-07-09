#pragma once
#include "component.h"
#include <raylib.h>
#include "consts.h"
#include "sprite.h"
#include "asset_manager.h"
#include <stdio.h>


void GraphicsComponent::Draw()
{
	SpriteSheet* ss = (SpriteSheet*)GetAssetManager()->GetAsset(sprite_id);
    
    if (ss)
    {
        Rectangle dest = { pos.x, pos.y, size.x, size.y };
        Rectangle frame = ss->GetFrame(frame_pos, flip_h, flip_v);
        
        DrawTexturePro(ss->texture, frame, dest, { origin.x, origin.y }, 0,
                       tint);
    }
    else
    {
        printf("SPRITESHEET WITH ID '%d' IS NULL\n", sprite_id);
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
