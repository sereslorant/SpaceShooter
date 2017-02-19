#ifndef L_RECT
#define L_RECT

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include "../lMath/lmVector2D.h"
#include "../lColor.h"

class lRect
{
public:
    enum ORIENTATION
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
    };
private:
    lmVector2D Position;
    float Width;
    float Height;

    ORIENTATION Orientation;

    lColor Color;
    GLuint Texture;

public:
    void SetPosition(const lmVector2D &position);
    void SetX(float X);
    void SetY(float Y);
    void SetWidth(float width);
    void SetHeight(float height);
    void SetOrientation(ORIENTATION orientation);
    void SetColor(const lColor &color);
    void SetTexture(GLuint texture);

    const lmVector2D &GetPosition() const;
    float GetWidth() const;
    float GetHeight() const;
    const lColor &GetColor(){return Color;}
    GLuint GetTexture()
    {
    	return Texture;
    }

    lRect(const lmVector2D &position,float width,float height,ORIENTATION orientation,const lColor &color,GLuint texture);
    ~lRect();

    void Draw();
};

#endif
