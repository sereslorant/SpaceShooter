#ifndef L_ENEMY_H
#define L_ENEMY_H

#include "lAgent.h"

class lEnemy : public lAgent
{
public:
    enum AGGRESSION
    {
        NONE,
        AGGRESSIVE,
        EVASIVE,
    };

    enum HORIZONTAL_MOVEMENT
    {
        H_MOVING_LEFT,
        H_MOVING_RIGHT,
        H_NOT_MOVING,
    };

    enum VERTICAL_MOVEMENT
    {
        V_MOVING_UP,
        V_MOVING_DOWN,
        V_NOT_MOVING,
    };

private:

    HORIZONTAL_MOVEMENT HorizontalMovement;
    VERTICAL_MOVEMENT   VerticalMovement;

    AGGRESSION Aggression;
    bool Shoot;

public:


    void SetHorizontalMovement(HORIZONTAL_MOVEMENT horizontal_movement)
    {
        HorizontalMovement = horizontal_movement;
    }

    void SetVerticalMovement(VERTICAL_MOVEMENT vertical_movement)
    {
        VerticalMovement = vertical_movement;
    }

    void SetAggression(AGGRESSION aggression)
    {
        Aggression = aggression;
    }

    void SetShoot(bool shoot)
    {
        Shoot = shoot;
    }

    void AvoidEnemyBullets(lMap *World);
    void ApproachNearestTarget(lMap *World);

    virtual void ControlAvatar(lMap *World,lRect *Viewport) override;
    lEnemy(lShip *avatar,AGGRESSION aggression);
};

#endif // L_ENEMY_H
