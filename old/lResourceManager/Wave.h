#ifndef WAVE
#define WAVE

#include <string>
#include <fstream>

#include <cmath>

#include "liWave.h"

/** \brief Egy .wav fájlból hullámformát betöltő class.
 *
 * \todo Be kell üzemelni, hogy két csatorna esetén mind a kettőt töltse be!
 *
 */
class lWave : public liWave
{
public:
	enum WAV_Error
	{
		WAV_ERR_COULDNT_OPEN_FILE = 0x0001,
		WAV_ERR_FILE_NOT_RECOGNIZED = 0x0002,
		WAV_ERR_FMT_FOUND = 0x0004,
		WAV_ERR_DATA_FOUND = 0x0008,
		WAV_ERR_UNSUPPORTED_FORMAT = 0x0010,

		WAV_ERR_OK = 12,
	};

private:
	unsigned short	Channels;
	unsigned int	Frequency;
	unsigned int	BitsPerSample;

	unsigned int Length;
	lWaveSample *Channel[2];

	unsigned int Error;

	void LoadFmt(std::ifstream &In);
	void LoadData(std::ifstream &In);

public:

	unsigned int GetFrequency() const
	{
		return Frequency;
	}

	unsigned int GetBitsPerSample() const
	{
		return BitsPerSample;
	}

	unsigned int GetLength() const
	{
		return Length;
	}

    unsigned int GetError() const
    {
    	return Error;
    }

	lWaveSample *GetChannel(unsigned int i)
	{
		if(i < 2)
		{
			return Channel[i];
		}
		else
		{
			return nullptr;
		}
	}

	const lWaveSample *GetChannel(unsigned int i) const
	{
		if(i < 2)
		{
			return Channel[i];
		}
		else
		{
			return nullptr;
		}
	}

	lWaveSample GetSample(unsigned int i,unsigned int channel) const
	{
		if((Channels == 1 && channel == 0) || (Channels == 2 && channel < 2))
		{
			return Channel[channel][i];
		}
		else
		{
			return 0.0;
		}
	}

	virtual unsigned int GetChannels() const override
	{
		return Channels;
	}

    virtual double GetPeriod() const override
    {
    	return 1.0/Frequency;
    }

    virtual double GetDuration() const override
    {
		return Length * GetPeriod();
    }

	virtual lWaveSample GetSample_t(double t,unsigned int channel) const override;

    void PrintError(std::ostream &out) const;

	lWave(const std::string &filename);
	virtual ~lWave() override;
};

std::ostream &operator<<(std::ostream &out,const lWave &wave);

#endif
