#ifndef L_STORE
#define L_STORE

#include "../../lResourceManager/RM.h"
#include "../lSpaceShooter/lShipDatabase.h"

#include "../lSSGameData.h"

#include "../../lInterface/lInput.h"

#include "../lMenu.h"
#include "../lwVerticalMenu.h"

class lHangar;

class lStore
{
private:
    enum lStoreMenuState
    {
        PLAYER_SELECT_MENU,
        STORE_MENU,
        SHIP_STORE_MENU,
        WEAPON_SLOT_MENU,
        WEAPON_STORE_MENU,
    };


    RM &ResourceManager;
    lShipDatabase &ShipDatabase;

    lSSGameData *GameData;

    lHangar *Game;

    lColor &ButtonColor;
    lColor &ActiveColor;

    lStoreMenuState StoreMenuState;

    //lPlayerShip *PlayerShip[N_PLAYERS]; //JAVÍTANI KELL!!!

    lMenu *PlayerSelectMenu;
    lwVerticalMenu *PlayerSelectMenuGraphics;

    //unsigned int &Cash;
    lRect *Ship;

    lMenu *StoreMenu;
    lwHorizontalMenu *StoreMenuGraphics;

    lMenu *ShipStoreMenu;
    lwWeirdMenu *ShipStoreMenuGraphics;

    std::map<std::string,lShipDatabase::ShipData>::iterator CurrentShip;
    lRect *CurrentShipModel;

    lMenu *WeaponSlotMenu[2];
    lwVerticalMenu *WeaponSlotMenuGraphics[2];

    lMenu *WeaponStoreMenu;
    lwWeirdMenu *WeaponStoreMenuGraphics;

    std::map<std::string,lShipDatabase::WeaponData>::iterator CurrentWeapon;
	
	const std::string &ContentDirectory;

    void SetPlayerShip(const std::string &ShipType);
    void DeletePlayerShip();
    void SetPlayerShipWeapon(unsigned int i,std::string WeaponType);
    void DeletePlayerShipWeapon(unsigned int i);
    void PrintCurrentShip();
    void PrintCurrentWeapon();
    void PrintPlayerShip();

public:
    lStore(RM &resource_manager,lShipDatabase &ship_database,lSSGameData *game_data,lHangar *game,lColor &button_color,lColor &active_color,const std::string &content_directory);
    ~lStore();

    void Prepare();

    void Logic(lInput &Input);
    void Draw();
};

#endif // L_STORE
