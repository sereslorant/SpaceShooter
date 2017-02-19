
#include "lHangar.h"


void lHangar::SetMenuState(int new_state)
{
    switch(new_state)
    {
    case MAIN_MENU:
        MenuState = MAIN_MENU;
        break;

    case MISSION:
        MenuState = MISSION;
        break;

    case STORE:
        MenuState = STORE;
        break;

    case LOAD_MENU:
        MenuState = LOAD_MENU;
        break;

    case SAVE_MENU:
        MenuState = SAVE_MENU;
        break;

    case OPTIONS:
        MenuState = OPTIONS;
        break;
    }
}

lHangar::lHangar(RM &resource_manager,lShipDatabase &ship_database,lSSGameData **game_data,std::string &next_map,float res_x,float res_y,const std::string &content_directory)
    :ResourceManager(resource_manager),ShipDatabase(ship_database),GameData(game_data),NextMap(next_map),ResX(res_x),ResY(res_y),FrameLimiter(60)
{
    MenuBg = new lRect(lmVector2D::NULL_VECTOR,ResX,ResY,lRect::UP,lColor(0.0,0.2,0.25,1.0),0);

    //Setting up main menu
    unsigned int nMenuOptions = 6;
    Menu = new lMenu(nMenuOptions);

    lmVector2D UpperLeft = lmVector2D(20.0,20.0);
    float Width = 200;
    float Height = 30;
    float Dist = 5;

    ButtonColor = lColor(0.0,0.4,0.5,1.0);
    ActiveColor = lColor(1.0,1.0,0.0,1.0);

    MenuGraphics = new lwVerticalMenu(Menu,UpperLeft,Width,Height,Dist,ButtonColor,ActiveColor);

    //Setting up load menu
    unsigned int nLoadMenuBtns = 11;
    LoadMenu = new lMenu(nLoadMenuBtns);

    lmVector2D LdMenuUpperLeft(250,20);
    float LdMenuWidth = 520;
    float LdMenuHeight = 47;
    float LdMenuDist = 5;

    LoadMenuGraphics = new lwVerticalMenu(LoadMenu,LdMenuUpperLeft,LdMenuWidth,LdMenuHeight,LdMenuDist,ButtonColor,ActiveColor);

    SolarSystemMap = new lSolarSystemMap(ResourceManager,*GameData,this,ButtonColor,ActiveColor,NextMap);
    Store = new lStore(ResourceManager,ShipDatabase,*GameData,this,ButtonColor,ActiveColor,content_directory);

    MenuState = MAIN_MENU;
}

lHangar::~lHangar()
{

    delete MenuBg;

    delete Menu;
    delete MenuGraphics;

    delete LoadMenu;
    delete LoadMenuGraphics;

    delete SolarSystemMap;
    delete Store;
}

int lHangar::MainLoop()
{
    int RetValue = 2;
    bool isRunning=true;

    Quit = false;
    while (isRunning)
    {
        //Input
        FrameLimiter.StartFrameLimiter();
        Input.CheckInput();

        //Logic
        if(Input.GetQuit() || Quit)
        {
            if(Input.GetQuit())
            {
                RetValue = -1;
            }
            isRunning = false;
        }

        switch(MenuState)
        {
        case MAIN_MENU:
            if(Input.GetUp())
            {
                Menu->GoUp();
                SDL_Delay(200);
            }
            if(Input.GetDown())
            {
                Menu->GoDown();
                SDL_Delay(200);
            }
            if(Input.GetEnter())
            {
                switch(Menu->GetActive())
                {
                case 0:
                    SolarSystemMap->Prepare();
                    MenuState = MISSION;
                    break;
                case 1:
                    MenuState = STORE;
                    Store->Prepare();
                    break;
                case 2:
                    MenuState = LOAD_MENU;
                    break;
                case 3:
                    MenuState = SAVE_MENU;
                    break;
                case 4:
                    MenuState = OPTIONS;
                    break;
                case 5:
                    isRunning = false;
                    RetValue = 1;
                    NextMap = "MainMenu";
                    break;
                }
                SDL_Delay(200);
            }
            MenuGraphics->Update();
            break;

        case MISSION:
            SolarSystemMap->Logic(Input,Quit,RetValue);
            break;

        case STORE:
            Store->Logic(Input);
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
                LoadMenu->SetActive(0);
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
                    NextMap = "Hangar";
                    RetValue = 1;
                }
            }

            LoadMenuGraphics->Update();

            break;

        case SAVE_MENU:
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
                LoadMenu->SetActive(0);
                SDL_Delay(200);
            }
            if(Input.GetEnter())
            {
                std::string filename = "Save_" + std::to_string(LoadMenu->GetActive());
                std::cout << filename << std::endl << LoadMenu->GetActive() << std::endl;;
                std::fstream fin;
                fin.open(filename.c_str());

                (*GameData)->Save(filename);
                SDL_Delay(200);
            }

            LoadMenuGraphics->Update();

            break;

        case OPTIONS:
            if(Input.GetEnter())
            {
                MenuState = MAIN_MENU;
                SDL_Delay(200);
            }
            break;
        }

        //Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0.0,ResX,ResY,0.0,-1,1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        MenuBg->Draw();

        MenuGraphics->Draw();

        switch(MenuState)
        {
        case MAIN_MENU:
            //Itt úgy néz ki, hogy semmit se csinálunk egyelőre. :P
            break;
        case MISSION:
            SolarSystemMap->Draw();
            break;

        case STORE:
            Store->Draw();
            break;

        case LOAD_MENU:
            LoadMenuGraphics->Draw();
            break;

        case SAVE_MENU:
            LoadMenuGraphics->Draw();
            break;

        case OPTIONS:

            break;
        }

        SDL_GL_SwapBuffers();

        FrameLimiter.CheckFrameLimiter();
    }

    return RetValue;
}
