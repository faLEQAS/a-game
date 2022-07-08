#pragma once
#include "consts.h"


struct Component
{
public:
		void (*Update)(Component*) = nullptr;
};


struct GraphicsComponent
{
public:
		int tic = 0; //the frame advances every 'x' tic
		int sprite_id = 0;
		//ALL IN PIXELS NOT IN METERS
		Vector2D pos = {};
		Vector2D size = {};
		Vector2D origin = {};
		Vector2D frame_pos = {};

		bool flip_h, flip_v = false;

		void Draw();
		void SetFlipOrigin();
		void NextSprite();
};