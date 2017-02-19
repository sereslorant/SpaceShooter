#ifndef L_TIMER_H
#define L_TIMER_H

#include "liTimer.h"

#include "lEvent.h"

class lTimer : public liTimer
{
private:
	bool Enabled;
	bool Repeat;
	double TimeElapsed;
	double TimeLimit;

	class lEnableTimerCallback : public liEventCallback
	{
	private:
		lTimer &Timer;

	public:

		virtual void Callback() override;

		lEnableTimerCallback(lTimer &timer);
		virtual ~lEnableTimerCallback() override;
	};

	class lDisableTimerCallback : public liEventCallback
	{
	private:
		lTimer &Timer;

	public:

		virtual void Callback() override;

		lDisableTimerCallback(lTimer &timer);
		virtual ~lDisableTimerCallback() override;
	};

	liEventCallback *EnableTimerCallback;
	liEventCallback *DisableTimerCallback;

	lEvent LimitReachedEvent;

public:

	void Enable();
	void Disable();

	bool IsEnabled();
	bool GetRepeat();

	double GetTimeLimit();
	void SetTimeLimit(double time_limit);
	double GetTimeElapsed();

	liEvent *GetLimitReachedEvent();
	liEventCallback *GetEnableTimerCallback();
	liEventCallback *GetDisableTimerCallback();

	void Step(double dt);

	lTimer(double time_limit,bool repeat = false);
	virtual ~lTimer() override;
};

#endif
