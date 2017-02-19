#ifndef LSDL2_FRAME_LIMITER
#define LSDL2_FRAME_LIMITER

#include "../lFrameLimiter.h"

#include "SDL/SDL.h"

class lSDL2FrameLimiter : public lFrameLimiter
{
private:
	const int MaxFPS;
	int Timer;
	//int FPS_Counter;
public:
	lSDL2FrameLimiter(int maxFPS);
    ~lSDL2FrameLimiter();

	void StartFrameLimiter() override;
	void CheckFrameLimiter() override;
};

#endif
