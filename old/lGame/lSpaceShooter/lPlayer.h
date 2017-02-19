#ifndef L_PLAYER_H
#define L_PLAYER_H

#include "lAgent.h"

class lPlayer : public lAgent
{
private:
    bool Up;
    bool Down;
    bool Left;
    bool Right;

    bool Shoot;
public:
    void UpdateInput(bool up,bool down,bool left,bool right,bool shoot);
    virtual void ControlAvatar(lMap *World,lRect *Viewport) override;

    lPlayer(lShip *avatar);
};

#endif // L_PLAYER_H
