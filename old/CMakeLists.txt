cmake_minimum_required(VERSION 2.6)
project(old)

set(CMAKE_CXX_STANDARD 11)

set(SS_GAME_DIR "./lGame")
set(HANGAR_DIR "${SS_GAME_DIR}/lHangar")
set(MAIN_MENU_DIR "${SS_GAME_DIR}/lMainMenu")
set(SPACE_SHOOTER_DIR "${SS_GAME_DIR}/lSpaceShooter")

set(HANGAR_SRC	"${HANGAR_DIR}/lHangar.cpp"
				"${HANGAR_DIR}/lSolarSystemMap.cpp"
				"${HANGAR_DIR}/lStore.cpp"
				)

set(MAIN_MENU_SRC	"${MAIN_MENU_DIR}/lMainMenu.cpp"
					)

set(SPACE_SHOOTER_SRC	"${SPACE_SHOOTER_DIR}/lAgent.cpp"
						"${SPACE_SHOOTER_DIR}/lEnemy.cpp"
						"${SPACE_SHOOTER_DIR}/lHUD.cpp"
						"${SPACE_SHOOTER_DIR}/lMap.cpp"
						"${SPACE_SHOOTER_DIR}/lObject.cpp"
						"${SPACE_SHOOTER_DIR}/lPlayer.cpp"
						"${SPACE_SHOOTER_DIR}/lProjectile.cpp"
						"${SPACE_SHOOTER_DIR}/lShip.cpp"
						"${SPACE_SHOOTER_DIR}/lSS_LuaInterface.cpp"
						)

set(SS_GAME_SRC	${HANGAR_SRC}
				${MAIN_MENU_SRC}
				${SPACE_SHOOTER_SRC}
				"${SS_GAME_DIR}/lCollision.cpp"
				"${SS_GAME_DIR}/lMenu.cpp"
				"${SS_GAME_DIR}/lRay.cpp"
				"${SS_GAME_DIR}/lRect.cpp"
				)

set(GAME_MODES_DIR	"./lGameModes")
set(GM_GAME_DIR		"${GAME_MODES_DIR}/lGame")
set(GM_AGENTS_DIR	"${GAME_MODES_DIR}/lGM_Agents")
set(AI_DIR			"${GM_AGENTS_DIR}/AI")
set(AI_COMMAND_DIR	"${AI_DIR}/lGM_AICommand")

set(GM_GAME_SRC	"${GM_GAME_DIR}/lEvent.cpp"
				"${GM_GAME_DIR}/lSoundSource.cpp"
				"${GM_GAME_DIR}/lTimer.cpp"
				)

set(AI_COMMAND_SRC	"${AI_COMMAND_DIR}/lGM_AICommand.cpp"
					"${AI_COMMAND_DIR}/lGM_AICommand_Goto.cpp"
					)

set(AI_SRC	${AI_COMMAND_SRC}
			"${AI_DIR}/lGM_AI.cpp"
			)

set(GM_AGENTS_SRC	${AI_SRC}
					"${GM_AGENTS_DIR}/lGM_Entity.cpp"
					"${GM_AGENTS_DIR}/lGM_Fighter.cpp"
					)

set(GAME_MODES_SRC	${GM_GAME_SRC}
					${GM_AGENTS_SRC}
					"${GAME_MODES_DIR}/lprotoMap.cpp"
					"${GAME_MODES_DIR}/lprotoObject.cpp"
					)

set(INTERFACE_DIR	"./lInterface")
set(SDL2_INTERFACE_DIR "${INTERFACE_DIR}/lSDL2Interface")

set(SDL2_INTERFACE_SRC	"${SDL2_INTERFACE_DIR}/lSDL2FrameLimiter.cpp"
						"${SDL2_INTERFACE_DIR}/lSDL2Input.cpp"
						"${SDL2_INTERFACE_DIR}/lSDL2SoundManager.cpp"
						"${SDL2_INTERFACE_DIR}/lSDLInit.cpp"
						)

set(INTERFACE_SRC	${SDL2_INTERFACE_SRC}
					"${INTERFACE_DIR}/lFrameLimiter.cpp"
					"${INTERFACE_DIR}/lInput.cpp"
					)

set(LUA_DIR	"./lLua")
set(LUA_SRC	"${LUA_DIR}/lapi.c"
			"${LUA_DIR}/lbitlib.c"
			"${LUA_DIR}/lctype.c"
			"${LUA_DIR}/ldo.c"
			"${LUA_DIR}/lgc.c"
			"${LUA_DIR}/llex.c"
			"${LUA_DIR}/loadlib.c"
			"${LUA_DIR}/loslib.c"
			"${LUA_DIR}/lstring.c"
			"${LUA_DIR}/ltablib.c"
			"${LUA_DIR}/lvm.c"
			"${LUA_DIR}/lauxlib.c"
			"${LUA_DIR}/lcode.c"
			"${LUA_DIR}/ldblib.c"
			"${LUA_DIR}/ldump.c"
			"${LUA_DIR}/linit.c"
			"${LUA_DIR}/lmathlib.c"
			"${LUA_DIR}/lobject.c"
			"${LUA_DIR}/lparser.c"
			"${LUA_DIR}/lstrlib.c"
			"${LUA_DIR}/ltm.c"
			"${LUA_DIR}/lzio.c"
			"${LUA_DIR}/lbaselib.c"
			"${LUA_DIR}/lcorolib.c"
			"${LUA_DIR}/ldebug.c"
			"${LUA_DIR}/lfunc.c"
			"${LUA_DIR}/liolib.c"
			"${LUA_DIR}/lmem.c"
			"${LUA_DIR}/lopcodes.c"
			"${LUA_DIR}/lstate.c"
			"${LUA_DIR}/ltable.c"
			"${LUA_DIR}/lundump.c"
			)

set(MATH_DIR	"./lMath")
set(MATH_SRC	"${MATH_DIR}/lMath.cpp"
				"${MATH_DIR}/lmGeometry2D.cpp"
				"${MATH_DIR}/lmVector2D.cpp"
				)

set(RM_DIR "./lResourceManager")
set(RM_SRC	"${RM_DIR}/BMP.cpp"
			"${RM_DIR}/Int.cpp"
			"${RM_DIR}/loadTexture.cpp"
			"${RM_DIR}/RM.cpp"
			"${RM_DIR}/Wave.cpp"
			)

set(SRC main.cpp lColor.cpp ${SS_GAME_SRC} ${GAME_MODES_SRC} ${INTERFACE_SRC} ${LUA_SRC} ${MATH_SRC} ${RM_SRC})

find_package(SDL)
find_package(OpenGL)

include_directories(${SDL_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})

add_executable(SpaceShooter_old ${SRC})
target_link_libraries(SpaceShooter_old ${SDL_LIBRARY} ${OPENGL_LIBRARIES})
