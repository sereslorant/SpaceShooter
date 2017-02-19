
#include "lEnemy.h"

#include "lShip.h"
#include "lMap.h"

using namespace std;

lEnemy::lEnemy(lShip *avatar,AGGRESSION aggression):lAgent(avatar)
{
    Aggression = aggression;
    Shoot = true;

    HorizontalMovement  = H_NOT_MOVING;
    VerticalMovement    = V_MOVING_DOWN; //V_NOT_MOVING;
}

void lEnemy::AvoidEnemyBullets(lMap *World)
{
    for(auto I = World->GetProjectiles().begin();I != World->GetProjectiles().end();I++)
    {
        if((*I)->GetSpecies() != Avatar->GetSpecies())
        {
            float Mlt;
            lmVector2D IPoint;
            if(CheckCollision(Avatar->GetRect(),(*I)->GetTrajectory(),&Mlt,&IPoint))
            {
                if(Mlt > 0.0)
                {
                    if(IPoint.X > Avatar->GetPosition().X)
                    {
                        HorizontalMovement = H_MOVING_LEFT;
                    }
                    else
                    {
                        HorizontalMovement = H_MOVING_RIGHT;
                    }

                    break;
                }
            }
        }
    }
}

void lEnemy::ApproachNearestTarget(lMap *World)
{
    bool Found = false;
    bool AlreadyInSight = false;
    std::list<lObject *>::const_iterator Nearest;

    for(auto I = World->GetObjects().begin();I != World->GetObjects().end();I++)
    {
        if((*I)->GetSpecies() >= 0 && ((*I)->GetSpecies() != Avatar->GetSpecies()))
        {
            if(!Found)
            {
                Nearest = I;
                Found = true;
            }
            else
            {
                if(((*I)->GetPosition() + (Avatar->GetPosition()*-1.0)).LengthSquared() < ((*Nearest)->GetPosition() + (Avatar->GetPosition()*-1.0)).LengthSquared())
                {
                    Nearest = I;
                }
            }

            for(unsigned int i=0;i < Avatar->GetWeaponsSize();i++)
            {
                const lShip::lWeaponSlot &Weapon_i = Avatar->GetWeapon(i);
                lRay Weapon_i_Trajectory(Avatar->GetPosition() + Weapon_i.Position,Weapon_i.Direction);

                float Mlt;
                if(Weapon_i.Active && CheckCollision((*I)->GetRect(),&Weapon_i_Trajectory,&Mlt))
                {
                    if(Mlt > 0.0)
                    {
                        AlreadyInSight = true;
                    }
                }
            }

            if(AlreadyInSight)
            {
                break;
            }
        }
    }

    if(Found && !AlreadyInSight)
    {
        if(Avatar->GetPosition().X < (*Nearest)->GetPosition().X)
        {
            HorizontalMovement = H_MOVING_RIGHT;
        }
        else
        {
            HorizontalMovement = H_MOVING_LEFT;
        }
    }
}

void lEnemy::ControlAvatar(lMap *World,lRect *Viewport)
{
    if(AvatarAlive)
    {
        switch(Aggression)
        {
        case NONE:

            break;
        case AGGRESSIVE:
            HorizontalMovement = H_NOT_MOVING;
            //VerticalMovement = V_NOT_MOVING;

            ApproachNearestTarget(World);
            break;
        case EVASIVE:
            HorizontalMovement = H_NOT_MOVING;
            //VerticalMovement = V_NOT_MOVING;

            AvoidEnemyBullets(World);
            break;
        }

        switch(HorizontalMovement)
        {
        case H_MOVING_LEFT:
            Avatar->SetMoveLeft(true);
            Avatar->SetMoveRight(false);
            break;
        case H_MOVING_RIGHT:
            Avatar->SetMoveRight(true);
            Avatar->SetMoveLeft(false);
            break;
        case H_NOT_MOVING:
            Avatar->SetMoveRight(false);
            Avatar->SetMoveLeft(false);
            break;
        }

        switch(VerticalMovement)
        {
        case V_MOVING_UP:
            Avatar->SetMoveUp(true);
            Avatar->SetMoveDown(false);
            break;
        case V_MOVING_DOWN:
            Avatar->SetMoveDown(true);
            Avatar->SetMoveUp(false);
            break;
        case V_NOT_MOVING:
            Avatar->SetMoveDown(false);
            Avatar->SetMoveUp(false);
            break;
        }

        if(CheckIfContains(Avatar->GetRect(),Viewport))
        {
            for(unsigned int i=0;i < Avatar->GetWeaponsSize();i++)
            {
                lProjectile *newProjectile;
                if(Avatar->SpawnProjectile(i,&newProjectile))
                {
                    World->AddProjectile(newProjectile);
                }
            }
        }
    }
}
