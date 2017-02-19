
#include "lHUD.h"
#include "lMap.h"

#include "lSpaceShooter.h"

lHUD::lBar::lBarData::lBarData()
{

}

lHUD::lBar::lBarData::lBarData(lmVector2D upperLeftCorner,float maxWidth,float height,lRect::ORIENTATION orientation,lColor color)
    :UpperLeftCorner(upperLeftCorner),MaxWidth(maxWidth),Height(height),Orientation(orientation),Color(color)
{

}

void lHUD::lBar::SetMaxValue(int maxValue)
{
    MaxValue = maxValue;
}

void lHUD::lBar::SetValue(int value)
{
    Value = value;
}

void lHUD::lBar::SetPosition(const lmVector2D &position)
{
    Quad->SetPosition(position);
}

void lHUD::lBar::SetMaxWidth(float maxWidth)
{
    MaxWidth = maxWidth;
}

void lHUD::lBar::UpdateWidth()
{
    float ScalingFactor = (float)Value / (float) MaxValue;
    Quad->SetWidth(ScalingFactor * MaxWidth);
}

void lHUD::lBar::Draw()
{
    Quad->Draw();
}

lHUD::lBar::lBar(const lBarData &BarData,GLuint texture)
    :MaxWidth(BarData.MaxWidth)//,Height(height)
{
    MaxValue    = 0;
    Value       = 0;

    Quad = new lRect(BarData.UpperLeftCorner,0.0,BarData.Height,BarData.Orientation,BarData.Color,texture);
}

lHUD::lBar::~lBar()
{
    delete Quad;
}

void lHUD::lPlayerStats::AttachShip(lShip *ship)
{
    Ship = ship;

    HealthBar.SetMaxValue(Ship->GetMaxHealth());
    ShieldBar.SetMaxValue(Ship->GetMaxShield());
}

void lHUD::lPlayerStats::DetachShip()
{
    Ship = nullptr;
}

void lHUD::lPlayerStats::Update()
{
    if(Ship != nullptr)
    {
        HealthBar.SetValue(Ship->GetHealth());
        ShieldBar.SetValue(Ship->GetShield());
    }
    else
    {
        HealthBar.SetValue(0);
        ShieldBar.SetValue(0);
    }

    HealthBar.UpdateWidth();
    ShieldBar.UpdateWidth();
}

void lHUD::lPlayerStats::Draw()
{
    HealthBar.Draw();
    ShieldBar.Draw();
}

lHUD::lPlayerStats::lPlayerStats(const lmVector2D &hposition,const lmVector2D &sposition,float maxWidth,float height,const lColor &hcolor,const lColor &scolor,GLuint htexture,GLuint stexture)
    :HealthBar(lBar::lBarData(hposition,maxWidth,height,lRect::UP,hcolor),htexture),ShieldBar(lBar::lBarData(sposition,maxWidth,height,lRect::UP,scolor),stexture)
{

}

void lHUD::AttachShip(unsigned int PlayerID,lShip *Player)
{
    PlayerStats[PlayerID]->AttachShip(Player);
}

void lHUD::DetachShip(unsigned int PlayerID)
{
    PlayerStats[PlayerID]->DetachShip();
}

void lHUD::Update()
{
    if(Game->PlayerIsAlive(0))
    {
        PlayerStats[0]->Update();
    }

    if(Game->IsCoop())
    {
        if(Game->PlayerIsAlive(1))
        {
            PlayerStats[1]->Update();
        }
    }
}

void lHUD::Logic(bool &Quit,int &RetValue)
{
    switch(Game->GetGameState())
    {
    case lSpaceShooter::IN_PROGRESS:
        if(Up == true)
        {
            PauseMenu->GoUp();

            SDL_Delay(200);
        }
        if(Down == true)
        {
            PauseMenu->GoDown();

            SDL_Delay(200);
        }
        if(Enter == true)
        {
            switch(PauseMenu->GetActive())
            {
            case 3:
                if(Game->IsCoop())
                {
                    Game->TurnOffCoop();
                }
                else
                {
                    Game->TurnOnCoop();
                }
            case 0:
                Game->Continue();
                SDL_Delay(200);
                break;
            case 4:
                Quit = true;
                RetValue = 1;
                MapName = "MainMenu";
                break;
            }
        }
        break;

    case lSpaceShooter::SUCCEEDED:
        if(Enter == true)
        {
            Quit = true;
            MapName = Game->GetNextMap();
            (*GameData)->AddCash(Game->GetEarnedPoints());
            RetValue = 1;
        }

        break;

    case lSpaceShooter::FAILED:
         if(Up == true)
        {
            FailMenu->GoUp();

            SDL_Delay(200);
        }
        if(Down == true)
        {
            FailMenu->GoDown();

            SDL_Delay(200);
        }

        if(Enter == true)
        {
            Quit = true;

            switch(FailMenu->GetActive())
            {
            case 0:
                RetValue = 1;
                break;

            case 1:
                RetValue = 1;
                MapName = "MainMenu";

                break;

            case 2:
                RetValue = -1;
                break;
            }
        }

        break;
    }

    PauseMenuGraphics->Update();
    FailMenuGraphics->Update();
}

void lHUD::Draw()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0,ResX,ResY,0.0,-1,1);

    glMatrixMode(GL_MODELVIEW);

    switch(Game->GetGameState())
    {
    case lSpaceShooter::IN_PROGRESS:
        if(Game->PlayerIsAlive(0))
        {
            PlayerStats[0]->Draw();
        }

        if(Game->IsCoop())
        {
            if(Game->PlayerIsAlive(1))
            {
                PlayerStats[1]->Draw();
            }
        }

        if(Game->IsPaused())
        {
            PauseMenuBg->SetWidth(ResX);
            PauseMenuBg->SetHeight(ResY);

            PauseMenuBg->Draw();

            PauseMenuGraphics->Draw();
        }
        break;

    case lSpaceShooter::SUCCEEDED:
        GameSuccessful->Draw();
        break;

    case lSpaceShooter::FAILED:
        GameFailed->Draw();

        FailMenuGraphics->Draw();
        break;
    }
}

lHUD::lHUD(lSpaceShooter *game,lSSGameData **game_data,std::string &map_name,float resX,float resY,const lmVector2D &BarOffset,float BarWidth,float BarHeight,float BarDist)
    :Game(game),GameData(game_data),MapName(map_name),ResX(resX),ResY(resY)
{
    PlayerStats[0] = new lPlayerStats(lmVector2D(ResX-(BarWidth+BarOffset.X),ResY-BarOffset.Y),lmVector2D(ResX-(BarWidth+BarOffset.X),ResY-(BarHeight+BarOffset.Y+BarDist)),BarWidth,BarHeight,lColor(0.0,1.0,0.0,1.0),lColor(0.0,1.0,1.0,1.0),0,0);
    PlayerStats[1] = new lPlayerStats(lmVector2D(BarOffset.X,ResY-BarOffset.Y),lmVector2D(BarOffset.X,ResY-(BarHeight+BarOffset.Y+BarDist)),BarWidth,BarHeight,lColor(0.0,1.0,0.0,1.0),lColor(0.0,1.0,1.0,1.0),0,0);

    PauseMenuBg = new lRect(lmVector2D::NULL_VECTOR,ResX,ResY,lRect::UP,lColor(0.0,0.0,0.0,0.6),0);

    unsigned int nMenuOptions = 5;
    PauseMenu = new lMenu(nMenuOptions);

    lColor ButtonColor = lColor(0.0,0.4,0.5,1.0);
    lColor ActiveColor = lColor(1.0,1.0,0.0,1.0);

    lmVector2D MenuUpperLeft = lmVector2D(20,20);
    float MenuWidth = 300;
    float MenuHeight = 45;
    float MenuDist = 5;

    PauseMenuGraphics = new lwVerticalMenu(PauseMenu,MenuUpperLeft,MenuWidth,MenuHeight,MenuDist,ButtonColor,ActiveColor);

    GameSuccessful = new lRect(lmVector2D((ResX - 400.0)/2.0,(ResY - 200.0)/2.0),400,200,lRect::UP,lColor(0.0,0.2,0.25,1.0),0);
    GameFailed = new lRect(lmVector2D((ResX - 400.0)/2.0,(ResY - 200.0)/2.0),400,200,lRect::UP,lColor(0.0,0.2,0.25,0.6),0);
    //Fail menu

    unsigned int nFailMenuOptions = 3;
    FailMenu = new lMenu(nFailMenuOptions);

    float FailMenuWidth = 160;
    float FailMenuHeight = 40;
    float FailMenuDist = 5;
    lmVector2D FailMenuUpperLeft  = lmVector2D((ResX - FailMenuWidth)/2.0,(ResY - FailMenuHeight * 1.2 * nFailMenuOptions)/2.0);

    FailMenuGraphics = new lwVerticalMenu(FailMenu,FailMenuUpperLeft,FailMenuWidth,FailMenuHeight,FailMenuDist,ButtonColor,ActiveColor);
}

lHUD::~lHUD()
{
    delete PlayerStats[0];
    delete PlayerStats[1];

    delete PauseMenuBg;
    delete PauseMenu;
    delete PauseMenuGraphics;

    delete GameSuccessful;

    delete GameFailed;
    delete FailMenu;
    delete FailMenuGraphics;
}
