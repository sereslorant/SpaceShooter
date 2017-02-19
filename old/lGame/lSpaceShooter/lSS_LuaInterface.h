#ifndef LSS_LUA_INTERFACE_H
#define LSS_LUA_INTERFACE_H

#include "../../lLua/lua.hpp"

int lLuaSS_GetCameraPos(lua_State *State);
int lLuaSS_GetDefPlayerSpeedX(lua_State *State);
int lLuaSS_GetDefPlayerSpeedY(lua_State *State);

int lLuaSS_SendMsg_MissionCompleted(lua_State *State);
int lLuaSS_SendMsg_MissionFailed(lua_State *State);

int lLuaSS_AddAgent(lua_State *State);
int lLuaSS_GetAvatarIsAlive(lua_State *State);
int lLuaSS_SetHorizontalMovement(lua_State *State);
int lLuaSS_SetVerticalMovement(lua_State *State);
int lLuaSS_SetAggression(lua_State *State);
int lLuaSS_SetShoot(lua_State *State);

#endif //LSS_LUA_INTERFACE_H
