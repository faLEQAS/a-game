#include "raycast_callback.h"
#include <stdio.h>

float RayCastCallback::ReportFixture(b2Fixture* B, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
    Object* object = (Object*)B->GetUserData().pointer;
    if (A)
    {
        return A->OnRayCastHit(object, B);
    }
    return 0;
}