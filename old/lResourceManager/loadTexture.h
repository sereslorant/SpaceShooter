#ifndef LOAD_TEXTURE_H
#define LOAD_TEXTURE_H

#include "BMP.h"

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

GLuint loadTexture(const std::string &filename);

#endif
