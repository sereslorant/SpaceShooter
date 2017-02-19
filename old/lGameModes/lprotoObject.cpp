
#include <cmath>

using namespace std;

#include"lprotoObject.h"
#include "lprotoCollisionCallback.h"
/*
int lprotoObject::GetSpecies()
{
	return Species;
}

bool lprotoObject::GetRemovable()
{
	return IsRemovable;
}
*/

const lmVector2D &lprotoObject::GetPosition()
{
	return Position;
}

const lmVector2D &lprotoObject::GetCenter()
{
	return Center;
}

const lmVector2D &lprotoObject::GetVelocity()
{
	return Velocity;
}

float lprotoObject::GetRadius()
{
	return Radius;
}

lprotoObject::COLLISION_RESPONSE lprotoObject::GetCollisionResponse()
{
	return CollisionResponse;
}

lprotoCollisionCallback *lprotoObject::GetCollisionCallback()
{
	return CollisionCallback;
}

void lprotoObject::SetPosition(const lmVector2D &position)
{
	Position = position;
}
void lprotoObject::SetCenter(const lmVector2D &center)
{
	Center = center;
}

void lprotoObject::SetVelocity(const lmVector2D &velocity)
{
	Velocity = velocity;
}
void lprotoObject::AddVelocity(const lmVector2D &dv)
{
	Velocity += dv;
}

void lprotoObject::SetRadius(float radius)
{
	Radius = radius;
}

void lprotoObject::GetCollisionResponse(COLLISION_RESPONSE collision_response)
{
	CollisionResponse = collision_response;
}

void lprotoObject::SetCollisionCallback(lprotoCollisionCallback *collision_callback)
{
	CollisionCallback = collision_callback;
}

void lprotoObject::DeleteCollisionCallback()
{
	delete CollisionCallback;
	CollisionCallback = nullptr;
}

lprotoObject::lprotoObject(const lmVector2D &position,const lmVector2D &center,float width,float height,const lColor &color,GLuint texture,float radius,/*int species,bool is_alive,bool is_targetable,*/COLLISION_RESPONSE collision_response,void *user_data)
	:/*Species(species),IsAlive(is_alive),IsTargetable(is_targetable),*/Position(position),Center(center),Radius(radius),CollisionResponse(collision_response),UserData(user_data)
{
	Removable = false;

	Rect = new lRect(position,width,height,lRect::UP,color,texture);

	CollisionCallback = nullptr;
}

lprotoObject::~lprotoObject()
{
	delete Rect;

	delete CollisionCallback;
}

void lprotoObject::Step(float dt)
{
	Position += Velocity * dt;
	Velocity = lmVector2D::NULL_VECTOR;

	Rect->SetX(Position.X - Center.X);
	Rect->SetY(Position.Y - Center.Y);
}

void lprotoObject::Draw()
{
	Rect->Draw();

	glLoadIdentity();

	glPointSize(5);
		glColor4f(1.0,0.0,0.0,1.0);

	glBegin(GL_POINTS);
		glVertex2f(Position.X,Position.Y);
	glEnd();

	glBegin(GL_LINE_LOOP);

	float PI = 3.14159265359;
	float dAngle = 2.0*PI / 8;

	for(int i=0;i < 8;i++)
	{
		glVertex2f(Position.X + Radius * cos(i*dAngle),Position.Y + Radius * sin(i*dAngle));
	}

	glEnd();
}
