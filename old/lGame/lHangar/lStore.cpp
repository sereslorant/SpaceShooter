
#include <iostream>

#include "lStore.h"
#include "lHangar.h"

using namespace std;

void lStore::SetPlayerShip(const std::string &ShipType)
{
    GameData->GetPlayerShip(PlayerSelectMenu->GetActive()) = new lSSGameData::lPlayerShip(ShipType,ShipDatabase.Ships[ShipType].nWeapons);
    Ship->SetTexture(ResourceManager.GetImage(ContentDirectory + ShipDatabase.Ships[GameData->GetPlayerShip(PlayerSelectMenu->GetActive())->GetShipType()].Texture));

    WeaponSlotMenu[PlayerSelectMenu->GetActive()] = new lMenu(GameData->GetPlayerShip(PlayerSelectMenu->GetActive())->WpnSlotSize());

    lmVector2D WpnSlotMenuUpperLeft(250,80);
    float WpnSlotMenuWidth = 250;
    float WpnSlotMenuHeight = 50;
    float WpnSlotMenuDist = 5;

    WeaponSlotMenuGraphics[PlayerSelectMenu->GetActive()] = new lwVerticalMenu(WeaponSlotMenu[PlayerSelectMenu->GetActive()],WpnSlotMenuUpperLeft,WpnSlotMenuWidth,WpnSlotMenuHeight,WpnSlotMenuDist,ButtonColor,ActiveColor);
}

void lStore::DeletePlayerShip()
{
    delete GameData->GetPlayerShip(PlayerSelectMenu->GetActive());

    delete WeaponSlotMenu[PlayerSelectMenu->GetActive()];
    delete WeaponSlotMenuGraphics[PlayerSelectMenu->GetActive()];
}

void lStore::SetPlayerShipWeapon(unsigned int i,std::string WeaponType)
{
    (*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[i].Active = true;
    (*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[i].Type = WeaponType;
}

void lStore::DeletePlayerShipWeapon(unsigned int i)
{
    (*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[i].Active = false;
}

void lStore::PrintCurrentShip()
{
    cout << "Cash: " << GameData->GetCash() << "\n";
    cout << "Speed: (" << CurrentShip->second.Speed.X << ","  << CurrentShip->second.Speed.Y << ")\n";
    cout << "MaxHp: " << CurrentShip->second.MaxHp << "\n";
    cout << "MaxShield: " << CurrentShip->second.MaxShield << "\n";
    cout << "Weapon slots: " << CurrentShip->second.nWeapons << "\n";
    cout << "PRICE: " << CurrentShip->second.Price << "\n\n";
}

void lStore::PrintCurrentWeapon()
{
    cout << "Cash: " << GameData->GetCash() << "\n";
    cout << "Speed: " << CurrentWeapon->second.Speed << "\n";
    cout << "Damage: " << CurrentWeapon->second.Damage << "\n";
    cout << "RechargeSpeed: " << CurrentWeapon->second.RechargeSpeed << "\n";
    cout << "PRICE: " << CurrentWeapon->second.Price << "\n\n";
}

void lStore::PrintPlayerShip()
{
    cout << "Cash: " << GameData->GetCash() << "\n";
    cout << "Ship type: " << GameData->GetPlayerShip(PlayerSelectMenu->GetActive())->GetShipType() << "\n";
    for(unsigned int i=0;i < GameData->GetPlayerShip(PlayerSelectMenu->GetActive())->WpnSlotSize();i++)
    {
        if(!(*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[i].Active)
        {
            cout << "Weapon slot " << i << ": empty\n";
        }
        else
        {
            cout << "Weapon slot " << i << ": " << (*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[i].Type << "\n";
        }
    }

}

lStore::lStore(RM &resource_manager,lShipDatabase &ship_database,lSSGameData *game_data,lHangar *game,lColor &button_color,lColor &active_color,const std::string &content_directory)
    :ResourceManager(resource_manager),ShipDatabase(ship_database),GameData(game_data),Game(game),ButtonColor(button_color),ActiveColor(active_color),ContentDirectory(content_directory)
{
    //PlayerShip[0] = Player0;
    //PlayerShip[1] = Player1;

    Ship = new lRect(lmVector2D(505.0,80.0),250.0,250.0,lRect::UP,lColor(1.0,1.0,1.0,1.0),0);
    Ship->SetTexture(ResourceManager.GetImage(ContentDirectory + ShipDatabase.Ships[GameData->GetPlayerShip(0)->GetShipType()].Texture));

    PlayerSelectMenu = new lMenu(lSSGameData::N_PLAYERS);

    lmVector2D PsMenuUpperLeft(250,20);
    float PsMenuWidth = 250;
    float PsMenuHeight = 50;
    float PsMenuDist = 5;

    PlayerSelectMenuGraphics = new lwVerticalMenu(PlayerSelectMenu,PsMenuUpperLeft,PsMenuWidth,PsMenuHeight,PsMenuDist,ButtonColor,ActiveColor);

    //Setting up load menu
    unsigned int nShopMenuButtons = 2;
    StoreMenu = new lMenu(nShopMenuButtons);

    lmVector2D ShMenuUpperLeft(250,20);
    float ShMenuWidth = 250;
    float ShMenuHeight = 50;
    float ShMenuDist = 5;

    StoreMenuGraphics = new lwHorizontalMenu(StoreMenu,ShMenuUpperLeft,ShMenuWidth,ShMenuHeight,ShMenuDist,ButtonColor,ActiveColor);

    ShipStoreMenu = new lMenu(ShipDatabase.Ships.size());

    lmVector2D ShipStMenuUpperLeft(250.0 + (505.0 - 300.0) / 2.0,420.0);
    float ShipStArrowWidth = 20;
    float ShipStArrowHeight = 50;

    float ShipStMenuWidth = 250;
    float ShipStMenuHeight = 50;
    float ShipStMenuDist = 5;

    ShipStoreMenuGraphics = new lwWeirdMenu(ShipStoreMenu,ShipStMenuUpperLeft,ShipStMenuWidth,ShipStMenuHeight,ShipStArrowWidth,ShipStArrowHeight,ShipStMenuDist,ButtonColor,ButtonColor,ActiveColor);

    CurrentShipModel = new lRect(lmVector2D(250.0,80.0),250.0,250.0,lRect::UP,lColor(1.0,1.0,1.0,1.0),0);

    lmVector2D WpnSlotMenuUpperLeft(250,80);
    float WpnSlotMenuWidth = 250;
    float WpnSlotMenuHeight = 50;
    float WpnSlotMenuDist = 5;

    WeaponSlotMenu[0] = new lMenu(GameData->GetPlayerShip(0)->WpnSlotSize());
    WeaponSlotMenuGraphics[0] = new lwVerticalMenu(WeaponSlotMenu[0],WpnSlotMenuUpperLeft,WpnSlotMenuWidth,WpnSlotMenuHeight,WpnSlotMenuDist,ButtonColor,ActiveColor);

    WeaponSlotMenu[1] = new lMenu(GameData->GetPlayerShip(1)->WpnSlotSize());
    WeaponSlotMenuGraphics[1] = new lwVerticalMenu(WeaponSlotMenu[1],WpnSlotMenuUpperLeft,WpnSlotMenuWidth,WpnSlotMenuHeight,WpnSlotMenuDist,ButtonColor,ActiveColor);

    WeaponStoreMenu = new lMenu(ShipDatabase.Weapons.size());
    WeaponStoreMenuGraphics = new lwWeirdMenu(WeaponStoreMenu,ShipStMenuUpperLeft,ShipStMenuWidth,ShipStMenuHeight,ShipStArrowWidth,ShipStArrowHeight,ShipStMenuDist,ButtonColor,ButtonColor,ActiveColor);
}

lStore::~lStore()
{
    delete PlayerSelectMenu;
    delete PlayerSelectMenuGraphics;

    delete Ship;

    delete StoreMenu;
    delete StoreMenuGraphics;

    delete ShipStoreMenu;
    delete ShipStoreMenuGraphics;

    delete CurrentShipModel;

    delete WeaponSlotMenu[0];
    delete WeaponSlotMenuGraphics[0];

    delete WeaponSlotMenu[1];
    delete WeaponSlotMenuGraphics[1];

    delete WeaponStoreMenu;
    delete WeaponStoreMenuGraphics;
}

void lStore::Prepare()
{
    StoreMenuState = PLAYER_SELECT_MENU;
}

void lStore::Logic(lInput &Input)
{
    switch(StoreMenuState)
    {
    case PLAYER_SELECT_MENU:
        if(Input.GetUp())
        {
            PlayerSelectMenu->GoUp();
            Ship->SetTexture(ResourceManager.GetImage(ContentDirectory + ShipDatabase.Ships[GameData->GetPlayerShip(PlayerSelectMenu->GetActive())->GetShipType()].Texture));
            SDL_Delay(200);
        }
        if(Input.GetDown())
        {
            PlayerSelectMenu->GoDown();
            Ship->SetTexture(ResourceManager.GetImage(ContentDirectory + ShipDatabase.Ships[GameData->GetPlayerShip(PlayerSelectMenu->GetActive())->GetShipType()].Texture));
            SDL_Delay(200);
        }
        if(Input.GetEscape())
        {
            Game->SetMenuState(0/*MAIN_MENU*/);
            SDL_Delay(200);
        }
        if(Input.GetEnter())
        {
            StoreMenuState = STORE_MENU;

            PrintPlayerShip();

            SDL_Delay(200);
        }

        PlayerSelectMenuGraphics->Update();

        break;
    case STORE_MENU:
        if(Input.GetLeft())
        {
            StoreMenu->GoUp();
            SDL_Delay(200);
        }
        if(Input.GetRight())
        {
            StoreMenu->GoDown();
            SDL_Delay(200);
        }
        if(Input.GetEscape())
        {
            StoreMenuState = PLAYER_SELECT_MENU;
            SDL_Delay(200);
        }
        if(Input.GetEnter())
        {
            switch(StoreMenu->GetActive())
            {
            case 0:
                StoreMenuState = SHIP_STORE_MENU;
                ShipStoreMenu->SetActive(0);
                CurrentShip = ShipDatabase.Ships.begin();
                CurrentShipModel->SetTexture(ResourceManager.GetImage(ContentDirectory + CurrentShip->second.Texture));
                SDL_Delay(200);
                break;
            case 1:
                StoreMenuState = WEAPON_SLOT_MENU;
                SDL_Delay(200);
                break;
            }
        }

        StoreMenuGraphics->Update();

        break;
    case SHIP_STORE_MENU:
        if(Input.GetLeft())
        {
            if(ShipStoreMenu->GetActive() != 0)
            {
                ShipStoreMenu->GoUp();
                CurrentShip--;
                CurrentShipModel->SetTexture(ResourceManager.GetImage(ContentDirectory + CurrentShip->second.Texture));
                PrintCurrentShip();
            }
            SDL_Delay(200);
        }
        if(Input.GetRight())
        {
            if(ShipStoreMenu->GetActive() != (ShipStoreMenu->GetSize() - 1))
            {
                ShipStoreMenu->GoDown();
                CurrentShip++;
                CurrentShipModel->SetTexture(ResourceManager.GetImage(ContentDirectory + CurrentShip->second.Texture));
                PrintCurrentShip();
            }
            SDL_Delay(200);
        }
        if(Input.GetEscape())
        {
            StoreMenuState = STORE_MENU;
            SDL_Delay(200);
        }
        if(Input.GetEnter())
        {
            if(GameData->GetCash() >= CurrentShip->second.Price)
            {
                StoreMenuState = STORE_MENU;
                for(unsigned int i=0;i < GameData->GetPlayerShip(PlayerSelectMenu->GetActive())->WpnSlotSize();i++)
                {
                    if((*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[i].Active)
                    {
                        GameData->AddCash(0.9 * ShipDatabase.Weapons[(*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[i].Type].Price);
                    }
                }

                GameData->AddCash(0.9 * ShipDatabase.Ships[GameData->GetPlayerShip(PlayerSelectMenu->GetActive())->GetShipType()].Price);

                DeletePlayerShip();
                SetPlayerShip(CurrentShip->first);

                GameData->TakeCash(CurrentShip->second.Price);
            }
            else
            {
                cout << "Error: not enough cash\n";
            }

            SDL_Delay(200);
        }

        ShipStoreMenuGraphics->Update();

        break;
    case WEAPON_SLOT_MENU:
        if(Input.GetUp())
        {
            WeaponSlotMenu[PlayerSelectMenu->GetActive()]->GoUp();
            SDL_Delay(200);
        }
        if(Input.GetDown())
        {
            WeaponSlotMenu[PlayerSelectMenu->GetActive()]->GoDown();
            SDL_Delay(200);
        }
        if(Input.GetEscape())
        {
            StoreMenuState = STORE_MENU;
            SDL_Delay(200);
        }
        if(Input.GetEnter())
        {
            StoreMenuState = WEAPON_STORE_MENU;
            //CurrentWeaponSlot = WeaponSlotMenu->GetActive();
            WeaponStoreMenu->SetActive(0);
            CurrentWeapon = ShipDatabase.Weapons.begin();
            //CurrentShipModel->SetTexture(ResourceManager.GetImage(CurrentShip->second.Texture));
            SDL_Delay(200);
        }

        WeaponSlotMenuGraphics[PlayerSelectMenu->GetActive()]->Update();

        break;
    case WEAPON_STORE_MENU:
        if(Input.GetLeft())
        {
            if(WeaponStoreMenu->GetActive() != 0)
            {
                WeaponStoreMenu->GoUp();
                CurrentWeapon--;
                PrintCurrentWeapon();
                //CurrentShipModel->SetTexture(ResourceManager.GetImage(CurrentShip->second.Texture));
            }
            SDL_Delay(200);
        }
        if(Input.GetRight())
        {
            if(WeaponStoreMenu->GetActive() != (WeaponStoreMenu->GetSize() - 1))
            {
                WeaponStoreMenu->GoDown();
                CurrentWeapon++;
                PrintCurrentWeapon();
                //CurrentShipModel->SetTexture(ResourceManager.GetImage(CurrentShip->second.Texture));
            }

            SDL_Delay(200);
        }
        if(Input.GetEscape())
        {
            StoreMenuState = WEAPON_SLOT_MENU;
            SDL_Delay(200);
        }
        if(Input.GetEnter())
        {
            if(GameData->GetCash() >= CurrentWeapon->second.Price)
            {
                StoreMenuState = WEAPON_SLOT_MENU;

                if((*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[WeaponSlotMenu[PlayerSelectMenu->GetActive()]->GetActive()].Active)
                {
                    GameData->AddCash(0.9 * ShipDatabase.Weapons[(*GameData->GetPlayerShip(PlayerSelectMenu->GetActive()))[WeaponSlotMenu[PlayerSelectMenu->GetActive()]->GetActive()].Type].Price);
                }

                DeletePlayerShipWeapon(WeaponSlotMenu[PlayerSelectMenu->GetActive()]->GetActive());
                SetPlayerShipWeapon(WeaponSlotMenu[PlayerSelectMenu->GetActive()]->GetActive(),CurrentWeapon->first);

                GameData->TakeCash(CurrentWeapon->second.Price);
            }
            else
            {
                cout << "Error: not enough cash\n";
            }

            SDL_Delay(200);
        }

        WeaponStoreMenuGraphics->Update();

    }
}

void lStore::Draw()
{
    switch(StoreMenuState)
    {
    case PLAYER_SELECT_MENU:
        PlayerSelectMenuGraphics->Draw();
        Ship->Draw();
        break;

    case SHIP_STORE_MENU:
        CurrentShipModel->Draw();
        ShipStoreMenuGraphics->Draw();
        StoreMenuGraphics->Draw();
        Ship->Draw();
        break;

    case STORE_MENU:
        StoreMenuGraphics->Draw();
        Ship->Draw();
        break;

    case WEAPON_SLOT_MENU:
        StoreMenuGraphics->Draw();
        WeaponSlotMenuGraphics[PlayerSelectMenu->GetActive()]->Draw();
        Ship->Draw();
        break;

    case WEAPON_STORE_MENU:
        StoreMenuGraphics->Draw();
        WeaponStoreMenuGraphics->Draw();
        break;
    }
}
