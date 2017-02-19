#ifndef L_COLLISION
#define L_COLLISION

#include <cmath>

#include "lRay.h"
#include "lRect.h"

bool CheckCollision(const lRect *Q,const lRay *R,float *Dist = nullptr,lmVector2D *Intersection = nullptr);
bool CheckIfContains(const lRect *Rect,const lRect *Cont);

#endif
