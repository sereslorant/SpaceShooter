#ifndef LI_SOUND_H
#define LI_SOUND_H

class liSound
{
public:

	virtual void Step(double dt) = 0;
	virtual double Sample(int channel) = 0;

    liSound()
    {

    }

    virtual ~liSound()
    {

    }
};

#endif
