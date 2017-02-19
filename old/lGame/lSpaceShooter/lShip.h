#ifndef L_SHIP_H
#define L_SHIP_H

#include <vector>

#include "lObject.h"
#include "lAgent.h"

class lShip : public lObject
{
public:
    struct lWeaponSlot
    {
        lmVector2D Position;
        lmVector2D Direction;
        //lRay Trajectory;

        bool Active;
        float Ready;

        float Speed;
        float Damage;
        float RechargeSpeed;
    };

private:
    lmVector2D Speed;
    lmVector2D DefSpeed;

    const float MaxHealth;
    float Health;
    const float MaxShield;
    float Shield;
    float ShieldRegenRate;

    lAgent *Agent;

    std::vector<lWeaponSlot> Weapons;

    bool MoveUp;
    bool MoveDown;
    bool MoveLeft;
    bool MoveRight;

public:

    void AttachAgent(lAgent *agent);

    unsigned int GetWeaponsSize();
    void AssignWeapon(unsigned int WeaponId,float Speed,float Damage,float RechargeSpeed);
    const lWeaponSlot &GetWeapon(unsigned int i);
    bool SpawnProjectile(unsigned int WeaponId,lProjectile **Target);

    virtual void Update(float dt) override;
    virtual float GetCollisionDamage() override
    {
        return Health + Shield;
    }

    virtual void TakeDamage(float Damage) override;

    lShip(const lmVector2D &position,SPECIES species,const lmVector2D &speed,float maxHealth,float maxShield,float shieldRegenRate,float width,float height,const lColor &color,GLuint texture,unsigned int points,unsigned int nWeapons,lmVector2D WPos[],lmVector2D WDir[]);

    float GetMaxHealth();
    float GetHealth();

    float GetMaxShield();
    float GetShield();

    const lmVector2D &GetDefSpeed()
    {
        return DefSpeed;
    }


    bool GetMoveUp()
    {
        return MoveUp;
    }
    bool GetMoveDown()
    {
        return MoveDown;
    }
    bool GetMoveLeft()
    {
        return MoveLeft;
    }
    bool GetMoveRight()
    {
        return MoveRight;
    }

    void SetDefSpeed(const lmVector2D &def_speed)
    {
        DefSpeed = def_speed;
    }

    void SetDefSpeedX(float def_speed_x)
    {
        DefSpeed.X = def_speed_x;
    }

    void SetDefSpeedY(float def_speed_y)
    {
        DefSpeed.Y = def_speed_y;
    }

    void SetMoveUp(bool move_up)
    {
        MoveUp = move_up;
    }
    void SetMoveDown(bool move_down)
    {
        MoveDown = move_down;
    }
    void SetMoveLeft(bool move_left)
    {
        MoveLeft = move_left;
    }
    void SetMoveRight(bool move_right)
    {
        MoveRight = move_right;
    }
    /*
    void MoveUp(float dt);
    void MoveDown(float dt);
    void MoveLeft(float dt);
    void MoveRight(float dt);
    */
};

#endif // L_SHIP_H
