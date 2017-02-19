#ifndef L_OBJECT_H
#define L_OBJECT_H

#include "lProjectile.h"

class lObject
{
protected:
    lmVector2D Position;

    SPECIES Species;
    lRect *Rect;

    bool Alive;
    unsigned int Points;

    void MoveX(float dx);
    void MoveY(float dy);

public:
    lObject(const lmVector2D &position,SPECIES species,float width,float height,lRect::ORIENTATION orientation,const lColor &color,GLuint texture,unsigned int points);
    virtual ~lObject();

    const lmVector2D &GetPosition();
    SPECIES GetSpecies();

    lRect *GetRect();

    bool IsAlive();
    float GetWidth();
    float GetHeight();

    unsigned int GetPoints()
    {
        return Points;
    }

    virtual void Update(float dt) = 0;
    virtual float GetCollisionDamage() = 0;
    virtual void TakeDamage(float Damage) = 0;

    void UpdateRect();

    void Draw();

    //void MoveX(float dx);
    //void MoveY(float dy);
};

#endif // L_OBJECT_H
