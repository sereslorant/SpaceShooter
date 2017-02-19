
#include "lShip.h"

using namespace std;

void lShip::AttachAgent(lAgent *agent)
{
    Agent = agent;
}

unsigned int lShip::GetWeaponsSize()
{
    return Weapons.size();
}

void lShip::AssignWeapon(unsigned int WeaponId,float Speed,float Damage,float RechargeSpeed)
{
    if(WeaponId < Weapons.size())
    {
        Weapons[WeaponId].Damage        = Damage;
        Weapons[WeaponId].Speed         = Speed;
        Weapons[WeaponId].RechargeSpeed = RechargeSpeed;

        Weapons[WeaponId].Ready = 1.0;

        Weapons[WeaponId].Active = true;
    }
}

const lShip::lWeaponSlot &lShip::GetWeapon(unsigned int i)
{
    return Weapons[i];
}

bool lShip::SpawnProjectile(unsigned int WeaponId,lProjectile **Target)
{
    if(WeaponId < Weapons.size() && Weapons[WeaponId].Active == true && Weapons[WeaponId].Ready >= 1.0)
    {
        *Target = new lProjectile(Position + Weapons[WeaponId].Position,
                                  Weapons[WeaponId].Direction,
                                  Weapons[WeaponId].Speed,
                                  Weapons[WeaponId].Damage,
                                  Species);

        Weapons[WeaponId].Ready = 0.0;

        return true;
    }

    return false;
}

void lShip::Update(float dt)
{
    MoveX(dt*DefSpeed.X);
    MoveY(dt*DefSpeed.Y);

    if(MoveUp)
    {
        MoveY(-dt*Speed.Y);
    }
    if(MoveDown)
    {
        MoveY(dt*Speed.Y);
    }
    if(MoveLeft)
    {
        MoveX(-dt*Speed.X);
    }
    if(MoveRight)
    {
        MoveX(dt*Speed.X);
    }

    if(Shield < MaxShield)
    {
        Shield += ShieldRegenRate;
    }

    for(auto I = Weapons.begin();I != Weapons.end();I++)
    {
        if(I->Active != false && I->Ready < 1.0)
        {
            I->Ready += I->RechargeSpeed * dt;
        }
    }
}

void lShip::TakeDamage(float Damage)
{
    if(Shield > 0.0)
    {
        if(Shield < Damage)
        {
            float hDamage = Damage - Shield;
            Shield = 0.0;
            Health -= hDamage;
        }
        else
        {
            Shield -= Damage;
        }
    }
    else
    {
        Health -= Damage;
    }

    if(Health < 0.0)
    {
        Alive = false;

        if(Agent != nullptr)
        {
            Agent->KillAvatar();
        }
    }
}

lShip::lShip(const lmVector2D &position,SPECIES species,const lmVector2D &speed,float maxHealth,float maxShield,float shieldRegenRate,float width,float height,const lColor &color,GLuint texture,unsigned int points,unsigned int nWeapons,lmVector2D WPos[],lmVector2D WDir[])
    :lObject(position,species,width,height,lRect::UP,color,texture,points),Speed(speed),MaxHealth(maxHealth),Health(maxHealth),MaxShield(maxShield),Shield(maxShield),ShieldRegenRate(shieldRegenRate),Agent(nullptr),Weapons(nWeapons),
        MoveUp(false),MoveDown(false),MoveLeft(false),MoveRight(false)
{
    if(Species != PLAYER)
    {
        Rect->SetOrientation(lRect::DOWN);
    }
    else
    {
        Rect->SetOrientation(lRect::UP);
    }

    Alive = true;

    for(unsigned int i=0;i < Weapons.size();i++)
    {
        if(Species != PLAYER)
        {
            Weapons[i].Position  = WPos[i] * -1.0;
            Weapons[i].Direction = WDir[i] * -1.0;
        }
        else
        {
            Weapons[i].Position  = WPos[i];
            Weapons[i].Direction = WDir[i];
        }

        //Weapons[i].Damage = 10;
        //Weapons[i].Speed = 300;

        //Weapons[i].RechargeSpeed = 3.0;
        //Weapons[i].Ready = 1.0;

        Weapons[i].Active = false;
    }
}

float lShip::GetMaxHealth()
{
    return MaxHealth;
}

float lShip::GetHealth()
{
    return Health;
}

float lShip::GetMaxShield()
{
    return MaxShield;
}

float lShip::GetShield()
{
    return Shield;
}
/*
void lShip::MoveUp(float dt)
{
    MoveY(-dt*Speed.Y);
}
void lShip::MoveDown(float dt)
{
    MoveY(dt*Speed.Y);
}
void lShip::MoveLeft(float dt)
{
    MoveX(-dt*Speed.X);
}
void lShip::MoveRight(float dt)
{
    MoveX(dt*Speed.X);
}
*/
