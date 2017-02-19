
#include "lSDL2FrameLimiter.h"

lSDL2FrameLimiter::lSDL2FrameLimiter(int maxFPS): MaxFPS(maxFPS)
{
    Timer=0;
    //FPS_Counter=0;
}

lSDL2FrameLimiter::~lSDL2FrameLimiter()
{

}

void lSDL2FrameLimiter::StartFrameLimiter()
{
    Timer = SDL_GetTicks();
}

void lSDL2FrameLimiter::CheckFrameLimiter()
{
    /*
    FPS_Counter++;
    if (FPS_Counter>MaxFPS)
    FPS_Counter=0;
    */

    int Timer_End = (SDL_GetTicks()-Timer);
    if (Timer_End < 1000/MaxFPS)
    {
        SDL_Delay((1000/MaxFPS) - Timer_End);
    }
}
