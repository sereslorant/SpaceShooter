
#include "lSoundSource.h"

liEvent *lSoundSource::GetStartEvent()
{
	return &StartEvent;
}

liEvent *lSoundSource::GetFinishedEvent()
{
	return &FinishedEvent;
}

liEvent *lSoundSource::GetRepeatEvent()
{
	return &RepeatEvent;
}

void lSoundSource::Step(double dt)
{
	switch(SoundSrcState)
	{
	case IDLE:
		break;

	case STARTED:
		TimeElapsed = 0.0;
		SoundSrcState = RUNNING;
		StartEvent.Fire();
		break;

	case RUNNING:

		if(Enabled)
		{
			TimeElapsed += dt;

			if(TimeElapsed > Wave->GetDuration())
			{
				if(Repeat)
				{
					SoundSrcState = STARTED;
					RepeatEvent.Fire();
				}
				else
				{
					SoundSrcState = IDLE;
					FinishedEvent.Fire();
				}
			}
		}
	}
}

double lSoundSource::Sample(int channel)
{
	if(SoundSrcState == RUNNING && Enabled)
	{
		return Wave->GetSample_t(TimeElapsed,channel);
	}
	else
	{
		return 0.0;
	}
}

lSoundSource::lSoundSource(liWave *wave,bool enabled,bool repeat)
	:Wave(wave),Enabled(enabled),Repeat(repeat),SoundSrcState(IDLE),TimeElapsed(0.0),
	StartCallback(this),StopCallback(this),EnableCallback(this),DisableCallback(this)
{

}

lSoundSource::~lSoundSource()
{

}
