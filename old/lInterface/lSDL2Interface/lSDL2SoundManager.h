#ifndef LSDL2_SOUND_MANAGER_H
#define LSDL2_SOUND_MANAGER_H

#include <SDL2/SDL.h>

#include <list>

#include "../liSound.h"

class lSDL2SoundManager
{
private:
	double dt;
	float Volume = 0.2;

    std::list<liSound *> SoundSources;

public:

	float GetVolume()
	{
		return Volume;
	}

	void AddSoundSource(liSound *sound)
	{
		SoundSources.push_back(sound);
	}

    void Step();
    double Sample(int channel);

    lSDL2SoundManager();

    virtual ~lSDL2SoundManager();
};

#endif // LSDL2_SOUND_MANAGER_H
