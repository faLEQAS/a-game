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
	
	if (IsKeyDown(KEY_SPACE) && on_ground)
	{
		body->SetLinearVelocity(Vector2D(body->GetLinearVelocity().x, -7.0f));
		on_ground = false;
	}
    
    if (IsKeyDown(KEY_X))
    {
        attack_time = 6;
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


void Player::OnRayCastHit(Object* B, b2Fixture* fixture, float fraction)
{
    if (true)
    {
        printf("IT HIT OBJECT OF TYPE %d\n", -1);
    }
    return 0;
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
        else if (obj->type == ObjectType::GOOMBA)
        {
            Goomba* goomba = (Goomba*)obj;
        }
	}
    else if (obj->type == ObjectType::GOOMBA)
    {
        /*bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input,
        
        const b2Transform& transform, int32 childIndex) const override;*/
        
        
        
        if (attack_time > 0)
        {
            //attack the goomba
            Goomba* goomba = (Goomba*)obj;
            goomba->body->SetLinearVelocity({ 0, -10.0f});
        }
        else
        {
            //get attacked by the goomba
            body->SetLinearVelocity({ 0,
                                        -10.0f });
        }
    }
    
    if (attack_time > 0)
    {
        attack_time--;
    }
    
    
}


void Player::EndContact(Object* obj, b2Fixture* fixture)
{
	if (fixture->GetShape() == ground_sensor)
    {
        on_ground = false;
    }
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
        if ((GetTic() - dir_change_tic) >= 20)
        {
            dir *= -1;
            graphics.flip_h = !graphics.flip_h;
            dir_change_tic = GetTic();
        }
        else
        {
            
        }
    }
}


void Goomba::EndContact(Object* obj, b2Fixture* fixture)
{
    //just_changed_dir = false;
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
