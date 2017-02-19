
#include <iostream>

#include "lSolarSystemMap.h"
#include "lHangar.h"

using namespace std;

lSolarSystemMap::lSolarSystemMap(RM &resource_manager,lSSGameData *game_data,lHangar *game,lColor &button_color,lColor &active_color,std::string &next_map)
    :ResourceManager(resource_manager),GameData(game_data),Game(game),ButtonColor(button_color),ActiveColor(active_color),/*Missions(missions),*/NextMap(next_map)
{
    Menu = new lMenu(GameData->GetMissionsSize());

    lmVector2D MenuUpperLeft(250,20);
    float MenuWidth = 150;
    float MenuHeight = 30;
    float MenuDist = 5;

    float ArrowWidth = 10;
    float ArrowHeight = 30;

    MenuGraphics = new lwWeirdMenu(Menu,MenuUpperLeft,MenuWidth,MenuHeight,ArrowWidth,ArrowHeight,MenuDist,ButtonColor,ButtonColor,ActiveColor);

    lmVector2D DescUpperLeft(250,60);
    float DescWidth = 180;
    float DescHeight = 250;

    DescriptionBg = new lRect(DescUpperLeft,DescWidth,DescHeight,lRect::UP,ButtonColor,0);

    lmVector2D SsmUpperLeft(440,20);
    float SsmSize = 350;

    SolarSystemMap = new lRect(SsmUpperLeft,SsmSize,SsmSize,lRect::UP,lColor(1.0,1.0,1.0,1.0),0);
}

lSolarSystemMap::~lSolarSystemMap()
{
    delete Menu;
    delete MenuGraphics;

    delete DescriptionBg;
    delete SolarSystemMap;
}

void lSolarSystemMap::Prepare()
{
    Menu->SetActive(0);
    CurrentMission = GameData->MissionsBegin();
}

void lSolarSystemMap::Logic(lInput &Input,bool &Quit,int &RetValue)
{
    if(Input.GetLeft())
    {
        if(Menu->GetActive() != 0)
        {
            Menu->GoUp();

            CurrentMission--;

            std::cout   << CurrentMission->Name << "\n"
                        << CurrentMission->Description << std::endl;

            SDL_Delay(200);
        }
    }
    if(Input.GetRight())
    {
        if(Menu->GetActive() != (Menu->GetSize()-1))
        {
            Menu->GoDown();

            CurrentMission++;

            std::cout   << CurrentMission->Name << "\n"
                        << CurrentMission->Description << std::endl;

            SDL_Delay(200);
        }
    }
    if(Input.GetEscape())
    {
        Game->SetMenuState(0/*MAIN_MENU*/);
    }
    if(Input.GetEnter())
    {
        NextMap = CurrentMission->MapName;
        cout << NextMap;
        Quit = true;
        RetValue = 1;
    }

    MenuGraphics->Update();
}

void lSolarSystemMap::Draw()
{
    SolarSystemMap->Draw();

    MenuGraphics->Draw();
    DescriptionBg->Draw();
}
