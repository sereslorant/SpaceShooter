#ifndef L_FRAME_LIMITER
#define L_FRAME_LIMITER

class lFrameLimiter
{
public:

	virtual void StartFrameLimiter() = 0;
	virtual void CheckFrameLimiter() = 0;

	lFrameLimiter();
    virtual ~lFrameLimiter();
};

#endif
