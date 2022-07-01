#include <box2d.h>
#include "object.h"


struct RayCastCallback : b2RayCastCallback
{
    Object* A = nullptr;
    float ReportFixture(b2Fixture* B, const b2Vec2& point,
                        const b2Vec2& normal, float fraction) override;
};


