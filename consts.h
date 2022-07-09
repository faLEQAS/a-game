#pragma once


#include <box2d.h>

#define COLLISION_DEBUG

typedef b2Vec2 Vector2D;

const float METER_TO_PIXEL_RATIO = 48;
const int MAX_OBJS = 300;

void DrawBody(b2Body& body, b2PolygonShape& shape, int thickness);
void GameOver();
int GetTic();
b2World* GetWorld();