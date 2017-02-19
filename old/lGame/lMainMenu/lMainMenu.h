#ifndef L_MAIN_MENU
#define L_MAIN_MENU

#include "../lGame.h"

#include "../lMenu.h"
#include "../lwVerticalMenu.h"

#include "../../lInterface/lSDL2Interface/lSDL2FrameLimiter.h"
#include "../../lInterface/lSDL2Interface/lSDL2Input.h"

#include "../../lResourceManager/RM.h"

#include "../lSSGameData.h"

#include <vector>
#include <string>

class lMainMenu : public lGame
{
private:

	RM &ResourceManager;
	lShipDatabase &ShipDatabase;

	lSSGameData **GameData;
    std::string &MapName;

    lSDL2Input Input;
	lSDL2FrameLimiter FrameLimiter;

    float ResX;
    float ResY;

    lRect *MenuBg;

    lMenu *Menu;
    lwVerticalMenu *MenuGraphics;

    lMenu *LoadMenu;
    lwVerticalMenu *LoadMenuGraphics;

    enum lMenuState
    {
        MAIN_MENU,
        LOAD_MENU,
        OPTIONS,
    };

	bool Quit;

	lMenuState MenuState;

public:
    lMainMenu(RM &resource_manager,lShipDatabase &ship_database,lSSGameData **game_data,std::string &map_name,float res_X,float res_Y);
    ~lMainMenu();

    int MainLoop();
};

#endif
