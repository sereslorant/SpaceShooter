#include <iostream>

#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "lInterface/lSDL2Interface/lSDLInit.h"


#include "lGame/lSpaceShooter/lSpaceShooter.h"
#include "lGame/lMainMenu/lMainMenu.h"
#include "lGame/lHangar/lHangar.h"
#include "lGameModes/lJRPG/lJRPG.h"
#include "lGameModes/lTD/lTD.h"


using namespace std;

class lProgram
{
private:
    std::string NextMap;
	
	std::string ContentDirectory = "Content/Images/";

    RM ResourceManager;
    lShipDatabase ShipDatabase;

    lSSGameData *GameData;

    void LoadGameMode(lGame **Game)
    {
        if(NextMap == "MainMenu")
        {
            *Game = new lMainMenu(ResourceManager,ShipDatabase,&GameData,NextMap,800,600);
        }
        else if(NextMap == "Hangar")
        {
            *Game = new lHangar(ResourceManager,ShipDatabase,&GameData,NextMap,800,600,ContentDirectory);
        }
        else if(NextMap == "JRPG")
        {
            *Game = new lJRPG(ResourceManager,NextMap,800,600);
        }
        else if(NextMap == "TowerDefense")
        {
            *Game = new lTD_GameMode(ResourceManager,NextMap,800,600);
        }
        else
        {
            *Game = new lSpaceShooter(ResourceManager,ShipDatabase,&GameData,NextMap,ContentDirectory);
        }
        /*
        break;

        default:
            cerr < "Error while loading game mode: game mode not found\n";
        }
        */
    }
public:
    lProgram()
        :NextMap("MainMenu"),ShipDatabase("Content/Game/Ships","Content/Game/Weapons"),GameData(nullptr)
    {

    }

    ~lProgram()
    {
        delete GameData;
    }

    void Start()
    {
        bool IsRunning = true;
        int RetValue = 2;

        lGame *Game;

        while(IsRunning)
        {
            bool Break = false;

            if(RetValue == -1)
            {
                IsRunning = false;
                Break = true;
            }
            else
            {
                LoadGameMode(&Game);
            }

            if(Break)
            {
                break;
            }

            RetValue = Game->MainLoop();

            delete Game;
        }
    }
};

int main(int argc,char *argv[])
{
    lSDLInit();

    lProgram Program;
	Program.Start();

	lSDLCleanup();

    return 0;
}
