
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "lSDLInit.h"

void lSDLInit()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_WM_SetCaption("Logan Spaceshooter prototype", NULL);
    SDL_SetVideoMode(800,600,32,SDL_OPENGL);

    glClearColor(0.0,0.0,0.0,1.0);
    glViewport(0,0,800,600);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(2);
}

void lSDLCleanup()
{
    SDL_Quit();
}
