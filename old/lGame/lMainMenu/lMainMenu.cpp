
#include "lMainMenu.h"

lMainMenu::lMainMenu(RM &resource_manager,lShipDatabase &ship_database,lSSGameData **game_data,std::string &map_name,float res_X,float res_Y)
    :ResourceManager(resource_manager),ShipDatabase(ship_database),GameData(game_data),MapName(map_name),FrameLimiter(60),ResX(res_X),ResY(res_Y)
{
    MenuBg = new lRect(lmVector2D::NULL_VECTOR,ResX,ResY,lRect::UP,lColor(0.0,0.2,0.25,1.0),0);

    MenuState = MAIN_MENU;

    //Setting up menu;
    unsigned int nMenuOptions = 4;
    Menu = new lMenu(nMenuOptions);

    lmVector2D MenuUpperLeft(30,30);
    float MenuWidth = 400;
    float MenuHeight = 40;
    float MenuDist = 5;
    lColor ButtonColor = lColor(0.0,0.4,0.5,1.0);
    lColor ActiveColor = lColor(1.0,1.0,0.0,1.0);

    MenuGraphics = new lwVerticalMenu(Menu,MenuUpperLeft,MenuWidth,MenuHeight,MenuDist,ButtonColor,ActiveColor);

    //Setting up load menu
    unsigned int nLoadMenuBtns = 11;
    LoadMenu = new lMenu(nLoadMenuBtns);

    lmVector2D LdMenuUpperLeft(30,10);
    float LdMenuWidth = 700;
    float LdMenuHeight = 47;
    float LdMenuDist = 5;

    LoadMenuGraphics = new lwVerticalMenu(LoadMenu,LdMenuUpperLeft,LdMenuWidth,LdMenuHeight,LdMenuDist,ButtonColor,ActiveColor);

    if(*GameData == nullptr)
    {
        *GameData = new lSSGameData(ShipDatabase);
    }
}

lMainMenu::~lMainMenu()
{
    delete Menu;
    delete MenuGraphics;

    delete LoadMenu;
    delete LoadMenuGraphics;

    delete MenuBg;
}

int lMainMenu::MainLoop()
{
    int RetValue = 0;
    bool isRunning=true;

    Quit = false;
    while (isRunning)
    {
        //Input
        FrameLimiter.StartFrameLimiter();
        Input.CheckInput();

        //Logic
        switch(MenuState)
        {
        case MAIN_MENU:

            if(Input.GetUp() == true)
            {
                Menu->GoUp();

                SDL_Delay(200);
            }
            if(Input.GetDown() == true)
            {
                Menu->GoDown();

                SDL_Delay(200);
            }
            if(Input.GetEnter() == true)
            {
                switch(Menu->GetActive())
                {
                case 0:
                    Quit = true;
                    MapName = "Hangar";
                    RetValue = 1;

                    //*GameData = new lSSGameData(ShipDatabase);
                    (*GameData)->SetDefault();

                    SDL_Delay(200);
                    break;
                case 1:
                    MenuState = LOAD_MENU;
                    SDL_Delay(200);
                    break;
                case 2:
                    MenuState = OPTIONS;
                    SDL_Delay(200);
                    break;
                case 3:
                    Quit = true;
                    RetValue = -1;
                    break;
                }
            }
            break;

        case LOAD_MENU:
            if(Input.GetUp())
            {
                LoadMenu->GoUp();

                SDL_Delay(200);
            }
            if(Input.GetDown())
            {
                LoadMenu->GoDown();

                SDL_Delay(200);
            }
            if(Input.GetEscape())
            {
                MenuState = MAIN_MENU;

                SDL_Delay(200);
            }
            if(Input.GetEnter())
            {
                std::string filename = "Save_" + std::to_string(LoadMenu->GetActive());
                std::fstream fin;
                fin.open(filename.c_str());
                if(fin.is_open())
                {
                    fin.close();

                    (*GameData)->Load(filename);

                    Quit = true;
                    MapName = "Hangar";
                    RetValue = 1;
                }
            }
            break;
        case OPTIONS:
            if(Input.GetEnter() == true)
            {
                MenuState = MAIN_MENU;
                SDL_Delay(200);
            }
            break;
        }

        if(Input.GetQuit() || Quit)
        {
            isRunning = false;
            if(Input.GetQuit())
            {
                RetValue = -1;
            }
        }

        MenuGraphics->Update();
        LoadMenuGraphics->Update();

        //Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0.0,ResX,ResY,0.0,-1,1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        MenuBg->Draw();

        switch(MenuState)
        {
        case MAIN_MENU:
            MenuGraphics->Draw();
            break;

        case LOAD_MENU:
            LoadMenuGraphics->Draw();
            break;

        case OPTIONS:
            //Itt most lófaszt se csinálunk, amíg be nem üzemelem az opciókat. xPPP
            //Ez a cucc csak azért van itt, hogy ne legyen compiler warning. :P
            break;
        }

        SDL_GL_SwapBuffers();

        FrameLimiter.CheckFrameLimiter();
    }

    return RetValue;
}

