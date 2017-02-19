#ifndef L_HUD_H
#define L_HUD_H

#include "lShip.h"
#include "lMap.h"
#include "../lMenu.h"
#include "../lwVerticalMenu.h"
#include "../../lInterface/lInput.h"

#include "../lSSGameData.h"

class lSpaceShooter;

class lHUD
{
private:
    struct lBar
    {
    private:
        int MaxValue;
        int Value;

        float MaxWidth;
        //float Height;

        lRect *Quad;

    public:

        struct lBarData
        {
            lmVector2D UpperLeftCorner;
            float MaxWidth;
            float Height;
            lRect::ORIENTATION Orientation;
            lColor Color;

            lBarData();
            lBarData(lmVector2D upperLeftCorner,float maxWidth,float height,lRect::ORIENTATION orientation,lColor color);
        };

        void SetMaxValue(int maxValue);
        void SetValue(int value);
        void SetPosition(const lmVector2D &position);

        void SetMaxWidth(float maxWidth);
        void UpdateWidth();

        void Draw();

        lBar(const lBarData &BarData,GLuint texture);
        ~lBar();
    };

    struct lPlayerStats
    {
        lBar HealthBar;
        lBar ShieldBar;

        lShip *Ship;

        void AttachShip(lShip *ship);
        void DetachShip();

        void Update();
        void Draw();

        lPlayerStats(const lmVector2D &hposition,const lmVector2D &sposition,float maxWidth,float height,const lColor &hcolor,const lColor &scolor,GLuint htexture,GLuint stexture);
    };
    //Input
    bool Up;
    bool Down;
    bool Enter;

    lSpaceShooter *Game;
    lSSGameData **GameData;
    std::string &MapName;

    float ResX;
    float ResY;

    lPlayerStats *PlayerStats[2];

    lRect *PauseMenuBg;

    lMenu *PauseMenu;
    lwVerticalMenu *PauseMenuGraphics;

    lRect *GameSuccessful;

    lRect *GameFailed;
    lMenu *FailMenu;
    lwVerticalMenu *FailMenuGraphics;

public:

    void UpdateInput(bool up,bool down,bool enter)
    {
        Up = up;
        Down = down;
        Enter = enter;
    }

    void AttachShip(unsigned int PlayerID,lShip *Player);
    void DetachShip(unsigned int PlayerID);

    void Update();

    void Logic(bool &Quit,int &RetValue);

    void Draw();

    lHUD(lSpaceShooter *game,lSSGameData **game_data,std::string &map_name,float resX,float resY,const lmVector2D &BarOffset = lmVector2D(20,40),float BarWidth = 160,float BarHeight = 12,float BarDist = 4);
    ~lHUD();
};

#endif // L_HUD_H
