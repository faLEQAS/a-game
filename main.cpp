#include <stdio.h>
#include <b2_world.h>
#include <b2_collision.h>
#include <b2_body.h>
#include <b2_polygon_shape.h>
#include <b2_fixture.h>
#include <raylib.h>
#include "contact_listener.h"
#include "asset_manager.h"
#include "consts.h"
#include "object.h"
#include "raycast_callback.h"

static bool collision_debug = true;
static AssetManager* asset_manager = nullptr;

static Object* objects[MAX_OBJS] = {};
static int tic = 0;


b2Body* CreateBody(b2World& world, b2BodyDef& def, b2Shape* fixtures, int fixture_count);
Player* CreatePlayer(b2World* world, Vector2D pos);
Tile* CreateTile(b2World* world, Vector2D pos, ObjectType type);
Goomba* CreateGoomba(b2World* world, Vector2D pos);
bool AddObj(Object* obj);
void Init();
void Destroy();
void Update(b2World* world, float timestep, uint32 velocity_iters, uint32 position_iters);
void Draw();


int GetTic()
{
	return tic;
}


AssetManager* GetAssetManager()
{
	return asset_manager;
}


Player* CreatePlayer(b2World* world, Vector2D pos)
{
	Player* object = new Player;
	object->type = ObjectType::PLAYER;
    
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.position = pos;
	def.gravityScale = 1;
	def.awake = true;
	def.fixedRotation = true;
	def.userData.pointer = (uintptr_t)object;
    
	b2PolygonShape shape;
	shape.SetAsBox(0.32f, 0.60f, Vector2D(0, -0.10f), 0);
	b2PolygonShape shape2;
	shape2.SetAsBox(0.3f, 0.1f, Vector2D(0, 0.65f), 0);
    
	b2FixtureDef fixdef;
	fixdef.shape = &shape;
	fixdef.density = 1.0f;
	fixdef.friction = 0.3f;
    
	b2FixtureDef fixdef2;
	fixdef2.shape = &shape2;
	fixdef2.density = 1.0f;
	fixdef2.friction = 0.3f;
    
    
	object->body = CreateBody(*world, def, nullptr, 0);
	object->box = (b2PolygonShape*)object->body->CreateFixture(&fixdef)->GetShape();
	object->ground_sensor = (b2PolygonShape*)object->body->CreateFixture(&fixdef2)->GetShape();
	
	object->graphics.size = { 48 * 2, 48 * 2 };
    object->graphics.sprite_id = SPRITE_PUNK_IDLE;
    
	AddObj(object);
    
	return object;
}


Tile* CreateTile(b2World* world, Vector2D pos, ObjectType type)
{
	Tile* object = new Tile;
	object->type = type;
	b2BodyDef def;
	def.type = b2_staticBody;
	def.position = pos;
	def.userData.pointer = (uintptr_t)object;
    
	//b2PolygonShape shape;
	//shape.SetAsBox(1.0f, 1.0f);
    
	b2EdgeShape edge1;
	edge1.SetTwoSided(Vector2D(-1.0f, -1.0f), Vector2D(1.0f, -1.0f));
    
	b2EdgeShape edge2;
	edge2.SetTwoSided(Vector2D(-1.0f, 1.0f), Vector2D(1.0f, 1.0f));
    
	b2EdgeShape edge3;
	edge3.SetTwoSided(Vector2D(-1.0f, -1.0f), Vector2D(-1.0F, 1.0f));
    
	b2EdgeShape edge4;
	edge4.SetTwoSided(Vector2D(1.0f, -1.0f), Vector2D(1.0f, 1.0f));
    
	object->body = CreateBody(*world, def, nullptr, 0);
	object->body->CreateFixture(&edge1, 0);
	object->body->CreateFixture(&edge2, 0);
	object->body->CreateFixture(&edge3, 0);
	object->body->CreateFixture(&edge4, 0);
    
	object->shape[0] = edge1;
	object->shape[1] = edge2;
	object->shape[2] = edge3;
	object->shape[3] = edge4;
    
	object->graphics.size = { 48 * 2, 48 * 2 };
	object->graphics.sprite_id = SPRITE_TILE;
    
	AddObj(object);
    
	return object;
}


Goomba* CreateGoomba(b2World* world, Vector2D pos)
{
	Goomba* object = new Goomba;
	object->type = ObjectType::GOOMBA;
    
	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.position = pos;
	def.gravityScale = 1;
	def.awake = true;
	def.fixedRotation = true;
	def.userData.pointer = (uintptr_t)object;
    
    b2PolygonShape shape;
	shape.SetAsBox(0.32f, 0.60f, Vector2D(0, -0.10f), 0);
	b2PolygonShape shape2;
	shape2.SetAsBox(0.3f, 0.1f, Vector2D(0, 0.65f), 0);
    
	b2FixtureDef fixdef;
	fixdef.shape = &shape;
	fixdef.density = 1.0f;
	fixdef.friction = 0.3f;
    fixdef.filter.groupIndex = -1;
    
	b2FixtureDef fixdef2;
	fixdef2.shape = &shape2;
	fixdef2.density = 1.0f;
	fixdef2.friction = 0.3f;
    fixdef2.filter.groupIndex = -1;
    
    
	object->body = CreateBody(*world, def, nullptr, 0);
    def.type = b2_kinematicBody;
    object->shadow_body = CreateBody(*world, def, nullptr, 0);
    object->box = (b2PolygonShape*)object->body->CreateFixture(&fixdef)->GetShape();
    object->shadow_box = (b2PolygonShape*)object->shadow_body->CreateFixture(&fixdef)->GetShape();
	
    object->ground_sensor = (b2PolygonShape*)object->body->CreateFixture(&fixdef2)->GetShape();
    
    object->graphics.sprite_id = SPRITE_PUNK_WALK;
    object->graphics.tic = 6;
	object->graphics.size = { 48 * 2, 48 * 2 };
    
	AddObj(object);
    
	return object;
}


int main(void)
{
	Init();
    
	b2Vec2 gravity = b2Vec2(0.0f, 8.0f);
	b2World world = b2World(gravity);
    
	ContactListener listener = ContactListener();
	world.SetContactListener(&listener);
    RayCastCallback raycast_callback = RayCastCallback();
    
	float timestep = 1.0f / 60.0f;
	uint32 velocity_iterations = 5;
	uint32 position_iterations = 8;
    
	SetTargetFPS(60);
    
	Player* p = CreatePlayer(&world, Vector2D(4.5f, 3.0f));
    Goomba* g = CreateGoomba(&world, Vector2D(2.5f, 0.0f));
    CreateTile(&world, Vector2D(1.0f, 4.0f), ObjectType::TILE);
	CreateTile(&world, Vector2D(1.0f, 6.0f), ObjectType::TILE);
	CreateTile(&world, Vector2D(3.0f, 6.0f), ObjectType::TILE);
	CreateTile(&world, Vector2D(5.0f, 6.0f), ObjectType::TILE);
    CreateTile(&world, Vector2D(7.0f, 6.0f), ObjectType::TILE);
	CreateTile(&world, Vector2D(9.0f, 6.0f), ObjectType::TILE);
	CreateTile(&world, Vector2D(11.0f, 6.0f), ObjectType::TILE);
	CreateTile(&world, Vector2D(15.0f, 6.0f), ObjectType::TILE_JUMP);
    
	while (!WindowShouldClose())
	{
        
		if (IsKeyDown(KEY_LEFT_CONTROL))
		{
			if (IsKeyPressed(KEY_C))
			{
				collision_debug = !collision_debug;
			}
		}
        
		Update(&world, timestep, velocity_iterations, position_iterations);
		Draw();
        
        Vector2D start = p->body->GetPosition();
        Vector2D end = Vector2D(start.x + 10.0f, start.y);
        
        Vector2 p1 = {start.x * METER_TO_PIXEL_RATIO, start.y * METER_TO_PIXEL_RATIO};
        Vector2 p2 = {end.x * METER_TO_PIXEL_RATIO,
            end.y * METER_TO_PIXEL_RATIO};
        
        world.RayCast(&raycast_callback, start, end);
        
        DrawLineEx(p1, p2, 4, YELLOW);
        
		tic++;
	}
    
	Destroy();
    
	return 0;
}


void Init()
{
	InitWindow(1280, 720, "Box2D");
    
	asset_manager = new AssetManager();
	asset_manager->LoadSpriteSheet(SPRITE_PUNK_IDLE, "assets/punk/idle.png", Vector2D(4, 1));
	asset_manager->LoadSpriteSheet(SPRITE_PUNK_WALK, "assets/punk/run.png", Vector2D(6, 1));
	asset_manager->LoadSpriteSheet(SPRITE_TILE, "assets/scene.png", Vector2D(1, 1));
}


void Destroy()
{
	CloseWindow();
    
	delete asset_manager;
}


void Update(b2World* world, float timestep, uint32 velocity_iters, uint32 position_iters)
{
	world->Step(timestep, velocity_iters, position_iters);
    
	for (int i = 0; i < MAX_OBJS; i++)
	{
		if (objects[i] && objects[i]->active)
		{
			objects[i]->Update();
		}
	}
}


void Draw()
{
	BeginDrawing();
	ClearBackground(BLACK);
    
	for (int i = 0; i < MAX_OBJS; i++)
	{
		if (objects[i] && objects[i]->active)
		{
			objects[i]->Draw();
		}
	}
    
	EndDrawing();
}


bool AddObj(Object* obj)
{
	for (int i = 0; i < MAX_OBJS; i++)
	{
		if (objects[i] == nullptr)
		{
			objects[i] = obj;
			return true;
		}
		else if (objects[i]->active == false)
		{
			delete objects[i];
			objects[i] = obj;
			return true;
		}
	}
	return false;
}


b2Body* CreateBody(b2World& world, b2BodyDef& def, b2Shape* fixtures, int fixture_count)
{
	b2Body* dynamic_body = world.CreateBody(&def);
    
	for (int i = 0; i < fixture_count; i++)
	{
		dynamic_body->CreateFixture(fixtures + i, 0);
	}
    
	return dynamic_body;
	
}


void DrawBody(b2Body& body, b2PolygonShape& shape, int thickness)
{
	if (shape.m_count == 4)
	{
		b2Vec2 bvec1 = body.GetWorldPoint(shape.m_vertices[0]);
		Vector2 vec1 = { bvec1.x * METER_TO_PIXEL_RATIO, bvec1.y * METER_TO_PIXEL_RATIO };
        
		b2Vec2 bvec2 = body.GetWorldPoint(shape.m_vertices[1]);
		Vector2 vec2 = { bvec2.x * METER_TO_PIXEL_RATIO, bvec2.y * METER_TO_PIXEL_RATIO };
        
		b2Vec2 bvec3 = body.GetWorldPoint(shape.m_vertices[2]);
		Vector2 vec3 = { bvec3.x * METER_TO_PIXEL_RATIO, bvec3.y * METER_TO_PIXEL_RATIO };
        
		b2Vec2 bvec4 = body.GetWorldPoint(shape.m_vertices[3]);
		Vector2 vec4 = { bvec4.x * METER_TO_PIXEL_RATIO, bvec4.y * METER_TO_PIXEL_RATIO };
        
#ifdef COLLISION_DEBUG
        
		{
			DrawLineEx(vec1, vec2, thickness, RED);
			DrawLineEx(vec2, vec3, thickness, RED);
			DrawLineEx(vec1, vec4, thickness, RED);
			DrawLineEx(vec3, vec4, thickness, RED);
		}
#endif
	}
	else if (shape.m_count == 3)
	{
		b2Vec2 bvec1 = body.GetWorldPoint(shape.m_vertices[0]);
		Vector2 vec1 = { bvec1.x * 64, bvec1.y * 64 };
        
		b2Vec2 bvec2 = body.GetWorldPoint(shape.m_vertices[1]);
		Vector2 vec2 = { bvec2.x * 64, bvec2.y * 64 };
        
		b2Vec2 bvec3 = body.GetWorldPoint(shape.m_vertices[2]);
		Vector2 vec3 = { bvec3.x * 64, bvec3.y * 64 };
        
#ifdef COLLISION_DEBUG
        
		{
			DrawLineEx(vec1, vec2, thickness, BLACK);
			DrawLineEx(vec2, vec3, thickness, BLACK);
			DrawLineEx(vec1, vec3, thickness, BLACK);
		}
#endif
	}
}