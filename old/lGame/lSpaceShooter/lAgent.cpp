
#include "lAgent.h"
#include "lShip.h"

lShip *lAgent::GetAvatar()
{
    return Avatar;
}

bool lAgent::AvatarIsKilled()
{
    return !AvatarAlive;
}

bool lAgent::AvatarIsAlive()
{
    return AvatarAlive;
}

void lAgent::KillAvatar()
{
    AvatarAlive = false;
}

lAgent::lAgent(lShip *avatar):Avatar(avatar),AvatarAlive(true)
{
    Avatar->AttachAgent(this);
}

lAgent::~lAgent()
{

}
