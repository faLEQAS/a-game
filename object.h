#pragma once
#include "consts.h"
#include "component.h"

/*
* when it comes to components, think of a b2Body as just a PhysicsComponent so we don't overcomplicate things and create
* a useless wrapper around b2Body for no reason
*/

enum ObjectType
{
	OBJECT_NONE,
	PLAYER,
	TILE,
	TILE_JUMP
};

struct Object
{
    public:
    ObjectType type = ObjectType::OBJECT_NONE;
    virtual void Update() {};
    virtual void Draw() {};
    virtual void StartContact(Object* obj, b2Fixture* fixture) {};
    virtual void EndContact(Object* obj, b2Fixture* fixture) {};
    
    bool active = true;
    Vector2D pos = {};
};


struct Player : public Object
{
    public:
    void Update() override;
    void Draw() override;
    void StartContact(Object* obj, b2Fixture* fixture) override;
    void EndContact(Object* obj, b2Fixture* fixture) override;
    
    b2Body* body = nullptr;
    b2PolygonShape* box = nullptr;
    bool on_ground = false;
    b2PolygonShape* ground_sensor = nullptr;
    
    GraphicsComponent component = GraphicsComponent();
};


struct Goomba : public Object
{
    public:
    void Update() override;
    void Draw() override;
    void StartContact(Object* obj, b2Fixture* fixture) override;
    void EndContact(Object* obj, b2Fixture* fixture) override;
    
    b2Body* body = nullptr;
    b2PolygonShape* box = nullptr;
    
    GraphicsComponent component = GraphicsComponent();
}


struct Tile : public Object
{
    public:
    void Update() override;
    void Draw() override;
    
    b2Body* body = nullptr;
    b2EdgeShape shape[4] = {};
    
    GraphicsComponent graphics = GraphicsComponent();
};