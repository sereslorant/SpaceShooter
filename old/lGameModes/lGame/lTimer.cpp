
#include "lTimer.h"

void lTimer::lEnableTimerCallback::Callback()
{
	Timer.Enable();
}

lTimer::lEnableTimerCallback::lEnableTimerCallback(lTimer &timer)
	:Timer(timer)
{

}

lTimer::lEnableTimerCallback::~lEnableTimerCallback()
{

}

void lTimer::lDisableTimerCallback::Callback()
{
	Timer.Disable();
}

lTimer::lDisableTimerCallback::lDisableTimerCallback(lTimer &timer)
	:Timer(timer)
{

}

lTimer::lDisableTimerCallback::~lDisableTimerCallback()
{

}

void lTimer::Enable()
{
	Enabled = true;
}

void lTimer::Disable()
{
	Enabled = false;
}

bool lTimer::IsEnabled()
{
	return Enabled;
}

bool lTimer::GetRepeat()
{
	return Repeat;
}

double lTimer::GetTimeLimit()
{
	return TimeLimit;
}

void lTimer::SetTimeLimit(double time_limit)
{
	TimeLimit = time_limit;
}

double lTimer::GetTimeElapsed()
{
	return TimeElapsed;
}

liEvent *lTimer::GetLimitReachedEvent()
{
	return &LimitReachedEvent;
}

liEventCallback *lTimer::GetEnableTimerCallback()
{
	return EnableTimerCallback;
}

liEventCallback *lTimer::GetDisableTimerCallback()
{
	return DisableTimerCallback;
}

void lTimer::Step(double dt)
{
	if(Enabled)
	{
		TimeElapsed += dt;

		if(TimeElapsed > TimeLimit)
		{
			LimitReachedEvent.Fire();

			//TimeElapsed -= TimeLimit;

			if(!Repeat)
			{
				Disable();
			}
			else
			{
				TimeElapsed -= TimeLimit;
			}
		}
	}
}

lTimer::lTimer(double time_limit,bool repeat)
	:Enabled(false),Repeat(repeat),TimeElapsed(0.0),TimeLimit(time_limit)
{
	EnableTimerCallback = new lEnableTimerCallback(*this);
	DisableTimerCallback = new lEnableTimerCallback(*this);
}

lTimer::~lTimer()
{
	delete EnableTimerCallback;
	delete DisableTimerCallback;
}
