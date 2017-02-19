#ifndef LI_TIMER_H
#define LI_TIMER_H

#include "liEvent.h"

class liTimer
{
public:

	virtual void Enable() = 0;
	virtual void Disable() = 0;
	virtual bool IsEnabled() = 0;
	virtual bool GetRepeat() = 0;
	virtual double GetTimeLimit() = 0;
	virtual void SetTimeLimit(double time_limit) = 0;
	virtual double GetTimeElapsed() = 0;

	virtual liEvent *GetLimitReachedEvent() = 0;
	virtual liEventCallback *GetEnableTimerCallback() = 0;
	virtual liEventCallback *GetDisableTimerCallback() = 0;

	virtual void Step(double dt) = 0;

	liTimer()
	{

	}

	virtual ~liTimer()
	{

	}
};

#endif // LI_TIMER_H
