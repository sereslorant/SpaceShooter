
#include "lPlayer.h"

#include "lShip.h"
#include "lMap.h"

void lPlayer::UpdateInput(bool up,bool down,bool left,bool right,bool shoot)
{
    Up      = up;
    Down    = down;
    Left    = left;
    Right   = right;

    Shoot   = shoot;
}

void lPlayer::ControlAvatar(lMap *World,lRect *Viewport)
{
    if(AvatarAlive)
    {
        //lRect  *Viewport = World->GetViewport();
        if(Up == true && Avatar->GetPosition().Y > (Viewport->GetPosition().Y + Avatar->GetHeight()/2.0))
        {
            Avatar->SetMoveUp(true);
        }
        else
        {
            Avatar->SetMoveUp(false);
        }

        if(Down == true && Avatar->GetPosition().Y < ((Viewport->GetPosition().Y + Viewport->GetHeight()) - Avatar->GetHeight()/2.0))
        {
            Avatar->SetMoveDown(true);
        }
        else
        {
            Avatar->SetMoveDown(false);
        }

        if(Left == true && Avatar->GetPosition().X > (Viewport->GetPosition().X + Avatar->GetWidth()/2.0))
        {
            Avatar->SetMoveLeft(true);
        }
        else
        {
            Avatar->SetMoveLeft(false);
        }

        if(Right == true && Avatar->GetPosition().X < ((Viewport->GetPosition().X + Viewport->GetWidth()) - Avatar->GetWidth()/2.0))
        {
            Avatar->SetMoveRight(true);
        }
        else
        {
            Avatar->SetMoveRight(false);
        }

        if(Shoot)
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

lPlayer::lPlayer(lShip *avatar):lAgent(avatar)
{

}
