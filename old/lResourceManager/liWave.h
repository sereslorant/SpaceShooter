#ifndef LI_WAVE_H
#define LI_WAVE_H

typedef float lWaveSample;

class liWave
{
public:
    virtual unsigned int GetChannels() const = 0;
    virtual double GetPeriod() const = 0;
    virtual double GetDuration() const = 0;

	virtual lWaveSample GetSample_t(double t,unsigned int channel) const = 0;

	liWave()
	{

	}

	virtual ~liWave()
	{

	}
};

#endif
