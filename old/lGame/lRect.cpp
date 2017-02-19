
#include "lRect.h"

void lRect::SetPosition(const lmVector2D &position)
{
    Position = position;
}

void lRect::SetX(float X)
{
    Position.X = X;
}

void lRect::SetY(float Y)
{
    Position.Y = Y;
}

void lRect::SetWidth(float width)
{
    Width = width;
}

void lRect::SetHeight(float height)
{
    Height = height;
}

void lRect::SetOrientation(lRect::ORIENTATION orientation)
{
    Orientation = orientation;
}

void lRect::SetColor(const lColor &color)
{
    Color = color;
}

void lRect::SetTexture(GLuint texture)
{
    Texture = texture;
}

const lmVector2D &lRect::GetPosition() const
{
    return Position;
}

float lRect::GetWidth() const
{
    return Width;
}

float lRect::GetHeight() const
{
    return Height;
}

lRect::lRect(const lmVector2D &position,float width,float height,ORIENTATION orientation,const lColor &color,GLuint texture)
    :Position(position),Width(width),Height(height),Orientation(orientation),Color(color),Texture(texture)
{

}

lRect::~lRect()
{

}

void lRect::Draw()
{
    glLoadIdentity();
    glTranslatef(Position.X,Position.Y,0.0);

    float Rotation;
    switch(Orientation)
    {
    case UP:
        Rotation = 0.0;
        break;

    case DOWN:
        Rotation = 180.0;
        break;

    case LEFT:
        Rotation = 270.0;
        break;

    case RIGHT:
        Rotation = 90.0;
        break;

    }

    glTranslatef(Width/2.0,Height/2.0,0.0);
    glRotatef(Rotation,0.0,0.0,1.0);
    glTranslatef(-Width/2.0,-Height/2.0,0.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glBegin(GL_QUADS);
    glColor4f(Color.Red,Color.Green,Color.Blue,Color.Alpha);

    glTexCoord2d(0,0); glVertex2f(0.0,0.0);
    glTexCoord2d(1,0); glVertex2f(Width,0.0);
    glTexCoord2d(1,1); glVertex2f(Width,Height);
    glTexCoord2d(0,1); glVertex2f(0.0,Height);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}
