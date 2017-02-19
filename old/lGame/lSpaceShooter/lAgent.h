#ifndef L_AGENT
#define L_AGENT

class lShip;
class lMap;

#include "../lRect.h"

class lAgent
{
protected:
    lShip *Avatar;
    bool AvatarAlive;

public:

    lShip *GetAvatar();
    bool AvatarIsKilled();
    bool AvatarIsAlive();
    void KillAvatar();

    virtual void ControlAvatar(lMap *World,lRect *Viewport) = 0;

    lAgent(lShip *avatar);
    virtual ~lAgent();
};

#endif // L_AGENT
