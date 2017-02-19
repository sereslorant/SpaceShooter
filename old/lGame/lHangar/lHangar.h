#ifndef L_HANGAR
#define L_HANGAR

#include "../../lResourceManager/RM.h"
#include "../lSpaceShooter/lShipDatabase.h"

#include "../../lMath/lmVector2D.h"

#include "../../lInterface/lSDL2Interface/lSDL2FrameLimiter.h"
#include "../../lInterface/lSDL2Interface/lSDL2Input.h"

#include "../lGame.h"

#include "../lMenu.h"
#include "../lwVerticalMenu.h"

#include "../lSSGameData.h"

#include "lSolarSystemMap.h"
#include "lStore.h"

class lHangar : public lGame
{
private:
    RM &ResourceManager;
    lShipDatabase &ShipDatabase;

    lSSGameData **GameData;
    std::string &NextMap;

    float ResX;
    float ResY;

    bool Quit;

    lSDL2FrameLimiter FrameLimiter;

    lSDL2Input Input;

    lColor ButtonColor;
    lColor ActiveColor;

    lRect *MenuBg;

    enum lMenuState
    {
        MAIN_MENU,
        MISSION,
        STORE,
        LOAD_MENU,
        SAVE_MENU,
        OPTIONS,
    };

    lMenuState MenuState;

    lMenu *Menu;
    lwVerticalMenu *MenuGraphics;

    lMenu *LoadMenu;
    lwVerticalMenu *LoadMenuGraphics;

    lSolarSystemMap *SolarSystemMap;
    lStore *Store;

public:

    void SetMenuState(int new_state);

    lHangar(RM &resource_manager,lShipDatabase &ship_database,lSSGameData **game_data,std::string &next_map,float res_x,float res_y,const std::string &content_directory);
    ~lHangar();

    int MainLoop();
};

#endif
