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
		component.flip_h = false;
		component.sprite_id = SPRITE_PUNK_WALK;
	}
	else if (IsKeyDown(KEY_LEFT))
	{
		body->SetLinearVelocity(Vector2D(-3.0f, body->GetLinearVelocity().y));
		component.flip_h = true;
		component.sprite_id = SPRITE_PUNK_WALK;
	}
	else
	{
		body->SetLinearVelocity(Vector2D(0, body->GetLinearVelocity().y));
		component.sprite_id = SPRITE_PUNK_IDLE;
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

	body->GetContactList();

	pos = body->GetWorldPoint(Vector2D(-0.5f, -1.25f));
}


void Player::Draw()
{
	//TODO(): See if you can extract a component out of this :) DONE()
	switch (component.sprite_id)
	{
		case SPRITE_PUNK_IDLE:
		{
			component.tic = 10;
		} break;

		case SPRITE_PUNK_WALK:
		{
			component.tic = 6;
		} break;
	}

	component.pos = { pos.x * METER_TO_PIXEL_RATIO, pos.y * METER_TO_PIXEL_RATIO };

	component.SetFlipOrigin();
	component.Draw();
	component.NextSprite();

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
