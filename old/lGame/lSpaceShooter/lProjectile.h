#ifndef L_PROJECTILE_H
#define L_PROJECTILE_H

#include "../lRay.h"
#include "../lRect.h"

enum SPECIES
{

    ASTEROID = -1,
    PLAYER,
    ENEMY,
};

class lProjectile
{
private:
    float Speed;
    float Damage;
    SPECIES Species;

    lRay *Trajectory;
    lRect *Rectangle;

public:

    float GetSpeed() const;
    float GetDamage() const;
    SPECIES GetSpecies() const;

    lRay *GetTrajectory();

    void Update(float dt);

    void Draw();

    lProjectile(const lmVector2D &position,const lmVector2D &direction,float speed,float damage,SPECIES species);
    ~lProjectile();
};

#endif //L_PROJECTILE_H
