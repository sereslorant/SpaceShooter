
#include "lObject.h"
using namespace std;

lObject::lObject(const lmVector2D &position,SPECIES species,float width,float height,lRect::ORIENTATION orientation,const lColor &color,GLuint texture,unsigned int points)
    :Position(position),Species(species),Points(points)
{
    Rect = new lRect(lmVector2D::NULL_VECTOR,width,height,orientation,color,texture);

    Alive = false;
}

lObject::~lObject()
{
    delete Rect;
}

const lmVector2D &lObject::GetPosition()
{
    return Position;
}

SPECIES lObject::GetSpecies()
{
    return Species;
}

lRect *lObject::GetRect()
{
    return Rect;
}

bool lObject::IsAlive()
{
    return Alive;
}

float lObject::GetWidth()
{
    return Rect->GetWidth();
}

float lObject::GetHeight()
{
    return Rect->GetHeight();
}

void lObject::UpdateRect()
{
    float avatarWidth = Rect->GetWidth()/2.0;
    float avatarHeight = Rect->GetHeight()/2.0;

    Rect->SetX(Position.X - avatarWidth);
    Rect->SetY(Position.Y - avatarHeight);
}

void lObject::Draw()
{
    Rect->Draw();
}

void lObject::MoveX(float dx)
{
    Position.X += dx;
}

void lObject::MoveY(float dy)
{
    Position.Y += dy;
}
