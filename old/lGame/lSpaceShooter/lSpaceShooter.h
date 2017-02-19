#ifndef L_SPACE_SHOOTER
#define L_SPACE_SHOOTER

#include "../../lMath/lmVector2D.h"

#include "../../lResourceManager/RM.h"

#include "../../lInterface/lSDL2Interface/lSDL2FrameLimiter.h"
#include "../../lInterface/lSDL2Interface/lSDL2Input.h"

#include "lShip.h"
#include "lPlayer.h"
#include "lEnemy.h"

#include "lMap.h"
#include "lHUD.h"

#include <map>

#include "../lGame.h"

#include "lShipDatabase.h"

#include "../lSSGameData.h"

//LUA
#include "../../lLua/lua.hpp"
#include "lSS_LuaInterface.h"

class lLuaVM
{
private:
    lua_State *State;
public:
    void SetGlobal(const std::string &name,void *light_user_data)
    {
        lua_pushlightuserdata(State,light_user_data);
        lua_setglobal(State,name.c_str());
    }

    void SetGlobal(const std::string &name,const std::string &str)
    {
        lua_pushstring(State,str.c_str());
        lua_setglobal(State,name.c_str());
    }

    void RegisterFunction(const std::string &name,int (*f) (lua_State *state))
    {
        lua_register(State,name.c_str(),f);
    }

    bool CallFunction(const std::string &name,int nArgs,int nRets)
    {
        lua_getglobal(State,name.c_str());
        if(lua_pcall(State,nArgs,nRets,0) != 0)
        {
            std::cerr << "Lua error: " << lua_tostring(State, -1) << std::endl;
            return false;
        }
        return true;
    }

    bool Init()
    {
        if(lua_pcall(State,0,0,0) != 0)
        {
            std::cerr << "Lua error: " << lua_tostring(State, -1) << std::endl;
            return false;
        }
        return true;
    }

    lLuaVM(const std::string &filename)
    {
        State = luaL_newstate();
        luaL_openlibs(State);
        luaL_loadfile(State,filename.c_str());
    }
    ~lLuaVM()
    {
        lua_close(State);
    }
};

class lSpaceShooter : public lGame
{
public:
    enum GAME_STATE
	{
	    IN_PROGRESS,
	    SUCCEEDED,
	    FAILED,
	};
private:
    ///System
	RM &ResourceManager;
    lShipDatabase &ShipDatabase;

	lSDL2Input Input;
	lSDL2FrameLimiter FrameLimiter;

	bool Quit;

	///Game
	lSSGameData **GameData;

    bool Coop;
    bool Paused;
    GAME_STATE GameState;

    unsigned int EarnedPoints;

    float dt;

    std::list<lAgent *> Agents;

    lPlayer *Player[2];
    lmVector2D DefPlayerSpeed;

    bool CameraMoving;
    lRect *Viewport;

    lMap *Map;
    lHUD *HUD;

    std::string &MapName;
    std::string NextMap;
	
	const std::string &ContentDirectory;

    //lua_State *State;

    lLuaVM *LuaVM;

    void Init()
    {
        float Width = 800;
        float Height = 600;

        //LoadShips("Ships");

        Map    = new lMap(this,Width,Height,ResourceManager,ContentDirectory,MapName);
        HUD    = new lHUD(this,GameData,MapName,Width,Height);

        Viewport = new lRect(lmVector2D::NULL_VECTOR,Width,Height,lRect::UP,lColor(1.0,1.0,1.0,1.0),0);

        //LUA
        LuaVM = new lLuaVM("Script/lGame.lua");
        //Game related
        LuaVM->RegisterFunction("lSS_GetCameraPos",lLuaSS_GetCameraPos);
        LuaVM->RegisterFunction("lSS_GetDefPlayerSpeedX",lLuaSS_GetDefPlayerSpeedX);
        LuaVM->RegisterFunction("lSS_GetDefPlayerSpeedY",lLuaSS_GetDefPlayerSpeedY);
        LuaVM->RegisterFunction("lSS_SendMsgMissionCompleted",lLuaSS_SendMsg_MissionCompleted);
        LuaVM->RegisterFunction("lSS_SendMsgMissionFailed",lLuaSS_SendMsg_MissionFailed);
        //Passing the game's pointer
        LuaVM->SetGlobal("pGame",this);
        //Passing the map's script file
        LuaVM->SetGlobal("MapScript","Map/"+ MapName +"/Script");

        //Agent related
        //Spawn agent
        LuaVM->RegisterFunction("lSS_SpawnAgent",lLuaSS_AddAgent);
        //Getter functions
        LuaVM->RegisterFunction("lSS_AvatarIsAlive",lLuaSS_GetAvatarIsAlive);
        //Control agent
        LuaVM->RegisterFunction("lSS_SetHorizontalMovement",lLuaSS_SetHorizontalMovement);
        LuaVM->RegisterFunction("lSS_SetVerticalMovement",lLuaSS_SetVerticalMovement);
        LuaVM->RegisterFunction("lSS_SetAggression",lLuaSS_SetAggression);
        LuaVM->RegisterFunction("lSS_SetShoot",lLuaSS_SetShoot);

        if(!LuaVM->Init())
        {
            std::cerr << "Error while running lua file" << std::endl;
        }

        //END

        Coop = false;
        Paused = false;

        GameState = IN_PROGRESS;

        dt = 1.0/60.0;

        DefPlayerSpeed = {0.0,-30.0};

        lShip *PlayerShip;

        NewShip(lmVector2D(460.0,500.0),PLAYER,(*GameData)->GetPlayerShip(0)->GetShipType(),&PlayerShip);

        for(unsigned int i=0;i < (*GameData)->GetPlayerShip(0)->WpnSlotSize();i++)
        {
            if((*(*GameData)->GetPlayerShip(0))[i].Active)
            {
                std::string WpnName = (*(*GameData)->GetPlayerShip(0))[i].Type;

                float Speed = ShipDatabase.Weapons[WpnName].Speed;
                float Damage = ShipDatabase.Weapons[WpnName].Damage;
                float RechargeSpeed = ShipDatabase.Weapons[WpnName].RechargeSpeed;

                std::cout << Speed << " " <<Damage << " " << RechargeSpeed << std::endl;

                PlayerShip->AssignWeapon(i,Speed,Damage,RechargeSpeed);
            }
        }

        PlayerShip->UpdateRect();
        PlayerShip->SetDefSpeed(DefPlayerSpeed);

        CameraMoving = true;

        PlayerShip->UpdateRect();
        HUD->AttachShip(0,PlayerShip);

        Player[0] = new lPlayer(PlayerShip);

        Map->AddObject(PlayerShip);
        AddAgent(Player[0]);
    }

    void DestroyGame()
    {
        delete LuaVM;

        delete HUD;
        delete Map;

        delete Viewport;

        for(auto I = Agents.begin();I != Agents.end();I++)
        {
            delete *I;
        }
    }

    void Logic()
    {
        if(Input.GetEscape())
        {
            Pause();
            SDL_Delay(200);
        }

        if(!LuaVM->CallFunction("MainLoop",0,0))
        {
            std::cerr << "Error while calling function " << "MainLoop" << std::endl;
        }

        for(auto I = Agents.begin();I != Agents.end();I++)
        {
            (*I)->ControlAvatar(Map,Viewport);
        }

        if(Coop && (Player[0]->AvatarIsKilled() && Player[1]->AvatarIsKilled()))
        {
            GameState = FAILED;
        }
        else if(!Coop && Player[0]->AvatarIsKilled())
        {
            GameState = FAILED;
        }

        Map->Step(dt,Viewport);

        if(CameraMoving)
        {
            Viewport->SetY(Viewport->GetPosition().Y + dt * DefPlayerSpeed.Y);
        }
    }

public:

    void AddPoints(unsigned int points)
    {
        EarnedPoints += points;
    }

    unsigned int GetEarnedPoints()
    {
        return EarnedPoints;
    }

    const std::string GetNextMap()
    {
        return NextMap;
    }

    void NewShip(const lmVector2D &Position,SPECIES Species,const std::string &ShipType,lShip **Target)
    {
        *Target = new lShip(Position,Species,
                            ShipDatabase.Ships[ShipType].Speed,
                            ShipDatabase.Ships[ShipType].MaxHp,
                            ShipDatabase.Ships[ShipType].MaxShield,
                            ShipDatabase.Ships[ShipType].ShieldRegenRate,
                            ShipDatabase.Ships[ShipType].Width,
                            ShipDatabase.Ships[ShipType].Height,
                            ShipDatabase.Ships[ShipType].Color,
                            ResourceManager.GetImage(ContentDirectory + ShipDatabase.Ships[ShipType].Texture),
                            ShipDatabase.Ships[ShipType].Points,
                            ShipDatabase.Ships[ShipType].nWeapons,&ShipDatabase.Ships[ShipType].WPos[0],&ShipDatabase.Ships[ShipType].WDir[0]);
    }

    GAME_STATE GetGameState()
    {
        return GameState;
    }

    void SendMissionCompleted()
    {
        if(GameState == IN_PROGRESS)
        {
            std::cout << "Mission completed" << std::endl;
            GameState = SUCCEEDED;
        }
    }
    void SendMissionFailed()
    {
        if(GameState == IN_PROGRESS)
        {
            std::cout << "Mission failed" << std::endl;
            GameState = FAILED;
        }
    }

    float GetCameraPos()
    {
        return Viewport->GetPosition().Y;
    }

    lmVector2D &GetDefPlayerSpeed()
    {
        return DefPlayerSpeed;
    }

    RM &GetRM()
    {
        return ResourceManager;
    }

    void AddAgent(lAgent *Agent)
    {
        Agents.push_back(Agent);
    }

    void DeleteAgent(lAgent *Agent)
    {
        for(auto I = Agents.begin();I != Agents.end();I++)
        {
            if(*I == Agent)
            {
                Agents.erase(I);
                break;
            }
        }
    }

    lMap *GetMap()
    {
        return Map;
    }

    const std::list<lAgent *> &GetAgents() const
    {
        return Agents;
    }

    bool PlayerIsAlive(unsigned int Id)
    {
        if(Id > 1)
        {
            return false;
        }

        return !Player[Id]->AvatarIsKilled();
    }

    void Pause()
    {
        Paused = true;
    }

    void Continue()
    {
        Paused = false;
    }

    bool IsPaused()
    {
        return Paused;
    }

    void TurnOnCoop()
    {
        Coop = true;

        lShip *PlayerShip;
        NewShip(Player[0]->GetAvatar()->GetPosition(),PLAYER,(*GameData)->GetPlayerShip(1)->GetShipType(),&PlayerShip);

        for(unsigned int i=0;i < (*GameData)->GetPlayerShip(1)->WpnSlotSize();i++)
        {
            if((*(*GameData)->GetPlayerShip(1))[i].Active)
            {
                std::string WpnName = (*(*GameData)->GetPlayerShip(1))[i].Type;

                float Speed = ShipDatabase.Weapons[WpnName].Speed;
                float Damage = ShipDatabase.Weapons[WpnName].Damage;
                float RechargeSpeed = ShipDatabase.Weapons[WpnName].RechargeSpeed;

                std::cout << Speed << " " <<Damage << " " << RechargeSpeed << std::endl;

                PlayerShip->AssignWeapon(i,Speed,Damage,RechargeSpeed);
            }
        }

        PlayerShip->UpdateRect();
        PlayerShip->SetDefSpeed(DefPlayerSpeed);

        HUD->AttachShip(1,PlayerShip);

        Player[1] = new lPlayer(PlayerShip);

        Map->AddObject(PlayerShip);
        AddAgent(Player[1]);

    }

    void TurnOffCoop()
    {
        Coop = false;
        HUD->DetachShip(1);

        Map->DeleteObject(Player[1]->GetAvatar());
        delete Player[1]->GetAvatar();

        DeleteAgent(Player[1]);
        delete Player[1];
        Player[1] = nullptr;
    }

    bool IsCoop()
    {
        return Coop;
    }

    lSpaceShooter(RM &resource_manager,lShipDatabase &ship_database,lSSGameData **game_data,std::string &map_name,const std::string &content_directory)
        :ResourceManager(resource_manager),ShipDatabase(ship_database),FrameLimiter(60),GameData(game_data),MapName(map_name),ContentDirectory(content_directory)//,ShipDatabase("Ships","Weapons")
    {
        std::string Path = "Map/" + map_name + "/MapData.data";
        std::fstream fin;
        fin.open(Path.c_str());

        if(!fin.is_open())
        {
            std::cerr << "Epic fail!\n";
            return;
        }

        while(!fin.eof())
        {
            std::string Tmp;
            fin >> Tmp;

            if(Tmp == "GameMode")
            {
                std::string GameMode;
                LoadQuotedString(fin,GameMode);

                if(GameMode != "SpaceShooter")
                {
                    std::cerr << "Epic fail 2\n";
                }
            }
            if(Tmp == "NextMap")
            {
                LoadQuotedString(fin,NextMap);
            }
        }
    }

    ~lSpaceShooter()
    {

    }

    int MainLoop()
    {
    	Init();

    	int RetValue = 0;
    	bool isRunning=true;

    	EarnedPoints = 0;

    	Quit = false;
		while (isRunning)
        {
			//Input
			FrameLimiter.StartFrameLimiter();
			Input.CheckInput();

			switch(GameState)
			{
            default:
                HUD->UpdateInput(Input.GetUp(),Input.GetDown(),Input.GetEnter());
                //RetVal = 1;

            case IN_PROGRESS:
                if(IsPaused())
                {
                    HUD->UpdateInput(Input.GetUp(),Input.GetDown(),Input.GetEnter());
                }
                else
                {
                    if(!Player[0]->AvatarIsKilled())
                    {
                        Player[0]->UpdateInput(Input.GetUp(),Input.GetDown(),Input.GetLeft(),Input.GetRight(),Input.GetRCtrl());
                    }

                    if(Coop)
                    {
                        if(!Player[1]->AvatarIsKilled())
                        {
                            Player[1]->UpdateInput(Input.GetW(),Input.GetS(),Input.GetA(),Input.GetD(),Input.GetLCtrl());
                        }
                    }
                }
			}

			//Logic
			if(Input.GetQuit() || Quit)
            {
                isRunning = false;

                if(Quit)
                {
                    RetValue = 0;
                }
                else
                {
                    RetValue = -1;
                }
            }

			switch(GameState)
			{
            default:
                HUD->Logic(Quit,RetValue);

            case IN_PROGRESS:
                if(IsPaused())
                {
                    if(Input.GetEscape())
                    {
                        Continue();
                        SDL_Delay(200);
                    }

                    HUD->Logic(Quit,RetValue);
                }
                else
                {
                    Logic();
                }

                HUD->Update();

                break;
			}

			//Rendering
			glClear(GL_COLOR_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glOrtho(Viewport->GetPosition().X,
                    Viewport->GetPosition().X + Viewport->GetWidth(),
                    Viewport->GetPosition().Y + Viewport->GetHeight(),
                    Viewport->GetPosition().Y,
                    -1,1);

            glMatrixMode(GL_MODELVIEW);

            Map->Draw();
            HUD->Draw();

            SDL_GL_SwapBuffers();

			FrameLimiter.CheckFrameLimiter();
        }

        DestroyGame();
        return RetValue;
    }
};

#endif //L_SPACE_SHOOTER
