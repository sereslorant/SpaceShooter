#ifndef LI_SOUND_SOURCE_H
#define LI_SOUND_SOURCE_H

#include "../../lInterface/liSound.h"

#include "liEvent.h"

class liSoundSource : public liSound
{
public:

	virtual liEventCallback *GetStartCallback() = 0;
	virtual liEventCallback *GetStopCallback() = 0;
	virtual liEventCallback *GetEnableCallback() = 0;
	virtual liEventCallback *GetDisableCallback() = 0;

	virtual liEvent *GetStartEvent() = 0;
    virtual liEvent *GetFinishedEvent() = 0;
    virtual liEvent *GetRepeatEvent() = 0;

    /*
	virtual void Step(double dt) = 0;
	virtual double Sample(int channel) = 0;
	*/

    liSoundSource()
    {

    }

    virtual ~liSoundSource()
    {

    }
};

#endif // LI_SOUND_SOURCE_H
