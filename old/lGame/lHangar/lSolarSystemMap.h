#ifndef L_SOLAR_SYSTEM_MAP
#define L_SOLAR_SYSTEM_MAP

#include "../../lResourceManager/RM.h"

#include "../lSSGameData.h"

#include "../../lInterface/lInput.h"

#include "../lMenu.h"
#include "../lwVerticalMenu.h"

class lHangar;

class lSolarSystemMap
{
private:
    RM &ResourceManager;

    lSSGameData *GameData;

    lHangar *Game;

    lColor &ButtonColor;
    lColor &ActiveColor;

    lMenu *Menu;
    lwWeirdMenu *MenuGraphics;

    lRect *DescriptionBg;
    lRect *SolarSystemMap;

    /*vector<lMission> &Missions;*/
    std::list<lSSGameData::lMission>::iterator CurrentMission;
    std::string &NextMap;

public:
    lSolarSystemMap(RM &resource_manager,lSSGameData *game_data,lHangar *game,lColor &button_color,lColor &active_color,std::string &next_map);

    ~lSolarSystemMap();

    void Prepare();

    void Logic(lInput &Input,bool &Quit,int &RetValue);

    void Draw();
};

#endif
