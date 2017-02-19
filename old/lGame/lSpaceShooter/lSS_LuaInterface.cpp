
#include "lSS_LuaInterface.h"

#include "lSpaceShooter.h"

#include <map>
#include <fstream>

using namespace std;

int lLuaSS_GetCameraPos(lua_State *State)
{
    lSpaceShooter *Game = (lSpaceShooter *)lua_touserdata(State,1);

    float CameraPos = Game->GetCameraPos();

    lua_pushnumber(State,CameraPos);

    return 1;
}


int lLuaSS_GetDefPlayerSpeedX(lua_State *State)
{
    lSpaceShooter *Game = (lSpaceShooter *)lua_touserdata(State,1);

    float PlayerSpeedX = Game->GetDefPlayerSpeed().X;

    lua_pushnumber(State,PlayerSpeedX);

    return 1;
}

int lLuaSS_GetDefPlayerSpeedY(lua_State *State)
{
    lSpaceShooter *Game = (lSpaceShooter *)lua_touserdata(State,1);

    float PlayerSpeedY = Game->GetDefPlayerSpeed().Y;

    lua_pushnumber(State,PlayerSpeedY);

    return 1;
}

int lLuaSS_SendMsg_MissionCompleted(lua_State *State)
{
    lSpaceShooter *Game = (lSpaceShooter *)lua_touserdata(State,1);
    Game->SendMissionCompleted();

    return 0;
}

int lLuaSS_SendMsg_MissionFailed(lua_State *State)
{
    lSpaceShooter *Game = (lSpaceShooter *)lua_touserdata(State,1);
    Game->SendMissionFailed();

    return 0;
}

int lLuaSS_AddAgent(lua_State *State)
{
    lSpaceShooter *Game = (lSpaceShooter *)lua_touserdata(State,1);

    float PosX = lua_tonumber(State,2);
    float PosY = lua_tonumber(State,3);

    int luaSpecies = lua_tointeger(State,4);

    SPECIES Species;
    switch(luaSpecies)
    {
    case 0:
        Species = PLAYER;
        break;
    case 1:
        Species = ENEMY;
    }

    std::string ShipType = lua_tostring(State,5);

    lShip *EnemyShip;
    Game->NewShip(lmVector2D(PosX,PosY),Species,ShipType,&EnemyShip);

    for(int i=0;i < EnemyShip->GetWeaponsSize();i++)
    {
        EnemyShip->AssignWeapon(i,300,10,3.0);
    }

    EnemyShip->UpdateRect();

    lmVector2D DefSpeed;

    DefSpeed.X = lua_tonumber(State,6);
    DefSpeed.Y = lua_tonumber(State,7);

    EnemyShip->SetDefSpeed(DefSpeed);

    int luaAggression = lua_tointeger(State,8);

    lEnemy::AGGRESSION Aggression;
    switch(luaAggression)
    {
    case lEnemy::EVASIVE:
        Aggression = lEnemy::EVASIVE;
        break;
    case lEnemy::AGGRESSIVE:
        Aggression = lEnemy::AGGRESSIVE;
        break;
    default:
        Aggression = lEnemy::NONE;
    }

    lAgent *EnemyAgent = new lEnemy(EnemyShip,Aggression);

    Game->GetMap()->AddObject(EnemyShip);
    Game->AddAgent(EnemyAgent);

    lua_pushlightuserdata(State,EnemyAgent);

    return 1;
}

int lLuaSS_GetAvatarIsAlive(lua_State *State)
{
    lEnemy *Agent = (lEnemy *)lua_touserdata(State,1);

    if(Agent->AvatarIsAlive())
    {
        lua_pushboolean(State,true);
    }
    else
    {
        lua_pushboolean(State,false);
    }

    return 1;
}

int lLuaSS_SetHorizontalMovement(lua_State *State)
{
    lEnemy *Agent = (lEnemy *)lua_touserdata(State,1);

    int luaHMovement = lua_tointeger(State,2);

    lEnemy::HORIZONTAL_MOVEMENT HMovement;
    switch(luaHMovement)
    {
    case lEnemy::H_MOVING_LEFT:
        HMovement = lEnemy::H_MOVING_LEFT;
        break;
    case lEnemy::H_MOVING_RIGHT:
        HMovement = lEnemy::H_MOVING_RIGHT;
        break;
    default:
        HMovement = lEnemy::H_NOT_MOVING;
    }

    Agent->SetHorizontalMovement(HMovement);

    return 0;
}

int lLuaSS_SetVerticalMovement(lua_State *State)
{
    lEnemy *Agent = (lEnemy *)lua_touserdata(State,1);

    int luaVMovement = lua_tointeger(State,2);

    lEnemy::VERTICAL_MOVEMENT VMovement;
    switch(luaVMovement)
    {
    case lEnemy::V_MOVING_UP:
        VMovement = lEnemy::V_MOVING_UP;
        break;
    case lEnemy::V_MOVING_DOWN:
        VMovement = lEnemy::V_MOVING_DOWN;
        break;
    default:
        VMovement = lEnemy::V_NOT_MOVING;
    }

    Agent->SetVerticalMovement(VMovement);

    return 0;
}

int lLuaSS_SetAggression(lua_State *State)
{
    lEnemy *Agent = (lEnemy *)lua_touserdata(State,1);

    int luaAggression = lua_tointeger(State,2);

    lEnemy::AGGRESSION Aggression;
    switch(luaAggression)
    {
    case lEnemy::EVASIVE:
        Aggression = lEnemy::EVASIVE;
        break;
    case lEnemy::AGGRESSIVE:
        Aggression = lEnemy::AGGRESSIVE;
        break;
    default:
        Aggression = lEnemy::NONE;
    }

    Agent->SetAggression(Aggression);

    return 0;
}

int lLuaSS_SetShoot(lua_State *State)
{
    lEnemy *Agent = (lEnemy *)lua_touserdata(State,1);

    int luaShooting = lua_toboolean(State,2);

    Agent->SetShoot(luaShooting);

    return 0;
}
