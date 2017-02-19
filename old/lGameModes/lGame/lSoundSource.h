#ifndef L_SOUND_SOURCE_H
#define L_SOUND_SOURCE_H

#include "liSoundSource.h"
#include "../../lResourceManager/liWave.h"

#include "lEvent.h"

class lSoundSource : public liSoundSource
{
private:
	class lStartSoundSource : public liEventCallback
	{
	private:
		lSoundSource *SoundSource;

	public:
		virtual void Callback() override
		{
			SoundSource->Start();
		}

		lStartSoundSource(lSoundSource *sound_source)
			:SoundSource(sound_source)
		{

		}

		virtual ~lStartSoundSource() override
		{

		}
	};

	class lStopSoundSource : public liEventCallback
	{
	private:
		lSoundSource *SoundSource;

	public:
		virtual void Callback() override
		{
			SoundSource->Stop();
		}

		lStopSoundSource(lSoundSource *sound_source)
			:SoundSource(sound_source)
		{

		}

		virtual ~lStopSoundSource() override
		{

		}
	};

	class lEnableSoundSource : public liEventCallback
	{
	private:
		lSoundSource *SoundSource;

	public:
		virtual void Callback() override
		{
			SoundSource->Enable();
		}

		lEnableSoundSource(lSoundSource *sound_source)
			:SoundSource(sound_source)
		{

		}

		virtual ~lEnableSoundSource() override
		{

		}
	};

	class lDisableSoundSource : public liEventCallback
	{
	private:
		lSoundSource *SoundSource;

	public:
		virtual void Callback() override
		{
			SoundSource->Disable();
		}

		lDisableSoundSource(lSoundSource *sound_source)
			:SoundSource(sound_source)
		{

		}

		virtual ~lDisableSoundSource() override
		{

		}
	};

	enum L_SOUND_SRC_STATE
	{
		IDLE,
		STARTED,
		RUNNING,
	};

    liWave *Wave;

    bool Enabled;
	bool Repeat;

	L_SOUND_SRC_STATE SoundSrcState;

	double TimeElapsed;

	lStartSoundSource	StartCallback;
	lStopSoundSource	StopCallback;
	lEnableSoundSource	EnableCallback;
	lDisableSoundSource	DisableCallback;

    lEvent StartEvent;
    lEvent FinishedEvent;
    lEvent RepeatEvent;

public:

	void Start()
	{
		if(SoundSrcState == IDLE)
		{
			SoundSrcState = STARTED;
		}
	}

	void Stop()
	{
		SoundSrcState = IDLE;
	}

	void Enable()
	{
		Enabled = true;
	}

	void Disable()
	{
		Enabled = false;
	}

	virtual liEventCallback *GetStartCallback() override
	{
		return &StartCallback;
	}

	virtual liEventCallback *GetStopCallback() override
	{
		return &StopCallback;
	}

	virtual liEventCallback *GetEnableCallback() override
	{
		return &EnableCallback;
	}

	virtual liEventCallback *GetDisableCallback() override
	{
		return &DisableCallback;
	}


	virtual liEvent *GetStartEvent() override;
    virtual liEvent *GetFinishedEvent() override;
    virtual liEvent *GetRepeatEvent() override;

	void Step(double dt);
	double Sample(int channel);

    lSoundSource(liWave *wave,bool enabled,bool repeat);
    virtual ~lSoundSource() override;
};

#endif // L_SOUND_SOURCE_H
