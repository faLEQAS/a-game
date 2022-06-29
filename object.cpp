#pragma once
#include "object.h"
#include "sprite.h"
#include "asset_manager.h"
#include <stdio.h>
#include "consts.h"


void Player::Update()
{
	if (IsKeyDown(KEY_RIGHT))
	{
		body->SetLinearVelocity(Vector2D(3.0f, body->GetLinearVelocity().y));
		graphics.flip_h = false;
		graphics.sprite_id = SPRITE_PUNK_WALK;
	}
	else if (IsKeyDown(KEY_LEFT))
	{
		body->SetLinearVelocity(Vector2D(-3.0f, body->GetLinearVelocity().y));
		graphics.flip_h = true;
		graphics.sprite_id = SPRITE_PUNK_WALK;
	}
	else
	{
		body->SetLinearVelocity(Vector2D(0, body->GetLinearVelocity().y));
		graphics.sprite_id = SPRITE_PUNK_IDLE;
	}
	if (body->GetLinearVelocity().y > 0)
	{
		on_ground = false;
	}
	if (IsKeyDown(KEY_SPACE) && on_ground)
	{
		body->SetLinearVelocity(Vector2D(body->GetLinearVelocity().x, -5.0f));
		on_ground = false;
	}
    
	pos = body->GetWorldPoint(Vector2D(-0.5f, -1.25f));
}


void Player::Draw()
{
	//TODO(): See if you can extract a component out of this :) DONE()
	switch (graphics.sprite_id)
	{
		case SPRITE_PUNK_IDLE:
		{
			graphics.tic = 10;
		} break;
        
		case SPRITE_PUNK_WALK:
		{
			graphics.tic = 6;
		} break;
	}
    
	graphics.pos = { pos.x * METER_TO_PIXEL_RATIO, pos.y * METER_TO_PIXEL_RATIO };
    
	graphics.SetFlipOrigin();
	graphics.Draw();
	graphics.NextSprite();
    
	//DrawBody(*body, *box, 3);
	//DrawBody(*body, *ground_sensor, 3);
}


void Player::StartContact(Object* obj, b2Fixture* fixture)
{
	if (fixture->GetShape() == ground_sensor)
	{
		on_ground = true;
		if (obj->type == TILE_JUMP)
		{
			body->SetLinearVelocity({ body->GetLinearVelocity().x, -5.0f });
        }
		//printf("DOWN\n");
	}
    else if (obj->type == ObjectType::GOOMBA)
    {
        body->SetLinearVelocity({ 0, -10.0f });
    }
}


void Player::EndContact(Object* obj, b2Fixture* fixture)
{
	//if (fixture->GetShape() == ground_sensor)
	//{
	//	on_ground = false;
	//	if (obj->type == TILE_JUMP)
	//	{
	//		body->SetLinearVelocity({ body->GetLinearVelocity().x, -5.0f });
	//	}
	//	//printf("DOWN\n");
	//}
}


void Goomba::Update()
{
    body->SetLinearVelocity({1.0f * dir ,body->GetLinearVelocity().y}
                            );
    pos = body->GetWorldPoint(Vector2D(-0.5f, -1.25f));
    shadow_body->SetLinearVelocity(body->GetLinearVelocity());
    shadow_body->SetTransform(body->GetPosition(), 0);
}


void Goomba::Draw()
{
    graphics.pos = {pos.x * METER_TO_PIXEL_RATIO, pos.y * METER_TO_PIXEL_RATIO};
    graphics.SetFlipOrigin();
    graphics.Draw();
    graphics.NextSprite();
    
    //DrawBody(*body, *box, 4);
    //DrawBody(*shadow_body, *shadow_box,4);
}


void Goomba::StartContact(Object* obj, b2Fixture* fixture)
{
    if (fixture->GetShape() != ground_sensor)
    {
        if (!just_changed_dir)
        {
            if (dir == 1)
            {
                dir = -1;
                just_changed_dir = true;
                graphics.flip_h = true;
            }
            else if (dir == -1)
            {
                dir = 1;
                just_changed_dir = true;
                graphics.flip_h = false;
            }
        }
    }
}


void Goomba::EndContact(Object* obj, b2Fixture* fixture)
{
    just_changed_dir = false;
}


void Tile::Update()
{
	pos = body->GetWorldPoint(Vector2D(-1.0f, -1.0f));
}


void Tile::Draw()
{
	//TODO(): See if you can extract a component out of this :) DONE()
    
	graphics.pos = { pos.x * METER_TO_PIXEL_RATIO, pos.y * METER_TO_PIXEL_RATIO };
	graphics.Draw();
    
    
#ifdef COLLISION_DEBUG
    
	for (int i = 0; i < 4; i++)
	{
		b2EdgeShape edge = shape[i];
        
		Vector2D v0 = body->GetWorldPoint(edge.m_vertex1);
		Vector2D v1 = body->GetWorldPoint(edge.m_vertex2);
        
		DrawLineEx({ v0.x * METER_TO_PIXEL_RATIO, v0.y * METER_TO_PIXEL_RATIO },
                   { v1.x * METER_TO_PIXEL_RATIO, v1.y * METER_TO_PIXEL_RATIO },
                   4,
                   BLUE);
	}
#endif
}
