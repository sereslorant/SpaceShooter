
#include <iostream>

#include "lSDL2SoundManager.h"

static void SDL2_SoundCallback(void *UserData,Uint8 *Stream8,int Length)
{
    lSDL2SoundManager *player = (lSDL2SoundManager *)UserData;
    Sint16 *Stream16 =  (Sint16 *)Stream8;

    unsigned int AmountOfSamples = Length/sizeof(Stream16[0]);

    for(int i=0;i < AmountOfSamples;i+=2)
    {
		Sint16 Sample = player->Sample(0) * 23767 * player->GetVolume();
		Stream16[i] = Sample;

		Sample = player->Sample(1) * 23767 * player->GetVolume();
		Stream16[i + 1] = Sample;

		player->Step();
    }
}

void lSDL2SoundManager::Step()
{
	for(liSound *SoundSource : SoundSources)
	{
		SoundSource->Step(dt);
	}
}

double lSDL2SoundManager::Sample(int channel)
{
	double Sample = 0.0;

	for(auto SoundSource : SoundSources)
	{
		Sample += SoundSource->Sample(channel);
	}

	return Sample;
}

lSDL2SoundManager::lSDL2SoundManager()
{
	SDL_AudioSpec AudioSpec;
    AudioSpec.freq = 48000;

    dt = 1.0/AudioSpec.freq;

	AudioSpec.format = AUDIO_S16LSB;

    AudioSpec.channels   = 2;
    AudioSpec.samples    = 4096;
    AudioSpec.callback   = SDL2_SoundCallback;
    AudioSpec.userdata   = this;

    if (SDL_OpenAudio(&AudioSpec, NULL) < 0)
	{
        std::cerr << "Couldn't open audio device\nError message: " <<  SDL_GetError() << std::endl;
    }

    SDL_PauseAudio(0);
}

lSDL2SoundManager::~lSDL2SoundManager()
{
	SDL_PauseAudio(1);

	for(auto SoundSource : SoundSources)
	{
		delete SoundSource;
	}
}
