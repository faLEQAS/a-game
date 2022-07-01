#include <box2d.h>



struct RayCastCallback : b2RayCastCallback
{
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                        const b2Vec2& normal, float fraction) override;
};


