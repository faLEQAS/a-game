#include "contact_listener.h"
#include "object.h"
#include <stdio.h>


void ContactListener::BeginContact(b2Contact* contact)
{
	Object* obj = (Object*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Object* obj2 = (Object*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	if (obj && obj2)
	{
		obj->StartContact(obj2, contact->GetFixtureA());
		obj2->StartContact(obj, contact->GetFixtureB());
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	Object* obj = (Object*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Object* obj2 = (Object*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	if (obj && obj2)
	{
		obj->EndContact(obj2, contact->GetFixtureA());
		obj2->EndContact(obj, contact->GetFixtureB());
	}
}
