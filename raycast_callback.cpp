#include "raycast_callback.h"
#include <stdio.h>

float RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
    printf("RAY HIT!\n");
    return fraction;
}