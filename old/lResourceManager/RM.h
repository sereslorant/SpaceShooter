#ifndef RM_H
#define RM_H

#include "loadTexture.h"

#include <vector>
#include <map>

#include "Wave.h"

class RM
{
private:
    std::vector<std::pair<std::string,GLuint> >LoadedImages;
    std::map<std::string,lWave *> LoadedWaves;

public:

	lWave *LoadWave(const std::string &filename)
	{
		auto I = LoadedWaves.find(filename);

		if(I == LoadedWaves.end())
		{
			lWave *NewWave = new lWave(filename);

			if(NewWave->GetError() != lWave::WAV_ERR_OK)
			{
				NewWave->PrintError(std::cerr);
				delete NewWave;

				return nullptr;
			}
			else
			{
				LoadedWaves[filename] = NewWave;
			}
		}

		return LoadedWaves[filename];
	}

	void FreeWaves()
	{
		for(auto I : LoadedWaves)
		{
			delete I.second;
		}
	}

    GLuint GetImage(const std::string &filename);
    ~RM();
};

void LoadQuotedString(std::istream &in,std::string &str);

#endif
