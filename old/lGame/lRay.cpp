
#include <cmath>

using namespace std;

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include "lRay.h"


lRay::lRay(const lmVector2D &position,const lmVector2D &direction)
    :Position(position),Direction(direction)
{
    Normal.X =  Direction.Y;
    Normal.Y = -Direction.X;

    Normal *= 1.0/sqrt(Normal.LengthSquared());
}

const lmVector2D &lRay::GetPosition() const
{
    return Position;
}

const lmVector2D &lRay::GetDirection() const
{
    return Direction;
}

const lmVector2D &lRay::GetNormal() const
{
    return Normal;
}

lmVector2D lRay::GetPoint(float l) const
{
    return Position + Direction * l;
}

void lRay::SetPosition(const lmVector2D &position)
{
    Position = position;
}

void lRay::SetDirection(const lmVector2D &direction)
{
    Direction = direction;

    Normal.X =  Direction.Y;
    Normal.Y = -Direction.X;

    Normal *= 1.0/sqrt(Normal.LengthSquared());
}

void lRay::Draw(float l)
{
    glLoadIdentity();

    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, Texture);

    glBegin(GL_LINES);
    //glColor4f(Color.Red,Color.Green,Color.Blue,Color.Alpha);
    glColor4f(1.0,0.0,0.0,1.0);

    glTexCoord2d(0,0); glVertex2f(Position.X,Position.Y);
    glTexCoord2d(0,0); glVertex2f(Position.X + l*Direction.X,Position.Y + l*Direction.Y);

    glColor4f(0.0,1.0,0.0,1.0);

    glTexCoord2d(0,0); glVertex2f(Position.X,Position.Y);
    glTexCoord2d(0,0); glVertex2f(Position.X + 20*Direction.X,Position.Y + 20*Direction.Y);

    glColor4f(0.0,0.0,1.0,1.0);

    glTexCoord2d(0,0); glVertex2f(Position.X,Position.Y);
    glTexCoord2d(0,0); glVertex2f(Position.X + 20*Normal.X,Position.Y + 20*Normal.Y);

    glEnd();
    //glDisable(GL_TEXTURE_2D);
}
