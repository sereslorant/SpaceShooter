#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "Int.h"
#include "Wave.h"

using namespace std;

#define FMT_LENGTH 16

void lWave::LoadFmt(ifstream &In)
{
    unsigned char Buffer[16];
    In.read((char *)Buffer,4);

    int Length;
    CombineInt_LittleEndian(Buffer,&Length);

    if(Length != FMT_LENGTH)
    {
        Error |= WAV_ERR_UNSUPPORTED_FORMAT;
        return;
    }

    In.read((char *)Buffer,Length);

    short AudioFormat;
	CombineShort_LittleEndian(Buffer,&AudioFormat);

	if(AudioFormat == 1)
	{
		//fread(Buffer,sizeof(char),2,File);
		CombineShort_LittleEndian(&Buffer[2],&Channels);

		//fread(Buffer,sizeof(char),4,File);
		CombineInt_LittleEndian(&Buffer[4],&Frequency);

		/*Getting rid of 4 bytes of unneccesary data*/
		//fread(Buffer,sizeof(char),4,File);
		/*Getting rid of 2 bytes of unneccesary data*/
		//fread(Buffer,sizeof(char),2,File);

		//fread(Buffer,sizeof(char),2,File);
		CombineShort_LittleEndian(&Buffer[14],&BitsPerSample);

        Error |= WAV_ERR_FMT_FOUND;
	}
	else
    {
        Error |= WAV_ERR_UNSUPPORTED_FORMAT;
    }
}

void lWave::LoadData(ifstream &In)
{
    unsigned char Buffer[4];

	In.read((char *)Buffer,4);

	unsigned int DataLength;
	CombineInt_LittleEndian(Buffer,&DataLength);

	if(BitsPerSample == 8)
    {
        Length = DataLength;
        unsigned char Tmp = 0;

        //Channel = new double[Length];

        if(Channels == 1)
		{
			Length = DataLength;

			Channel[0] = new lWaveSample[Length];
			Channel[1] = nullptr;
		}
		else if(Channels == 2)
		{
			Length = DataLength/2;

			Channel[0] = new lWaveSample[Length];
			Channel[1] = new lWaveSample[Length];
		}
		else
		{
			Error |= WAV_ERR_UNSUPPORTED_FORMAT;
			return;
		}

        for(unsigned int i=0;i <Length;i++)
        {
            In.read((char *)&Tmp,1);

            Channel[0][i] = (Tmp/255.0)-0.5;

            if(Channels == 2)
			{
				In.read((char *)&Tmp,1);

				Channel[1][i] = (Tmp/255.0)-0.5;
			}
        }

        Error |= WAV_ERR_DATA_FOUND;
    }
	else if(BitsPerSample == 16)
    {
        short Tmp = 0;

        if(Channels == 1)
		{
			Length = DataLength/sizeof(short);

			Channel[0] = new lWaveSample[Length];
			Channel[1] = nullptr;
		}
		else if(Channels == 2)
		{
			Length = DataLength/(2*sizeof(short));

			Channel[0] = new lWaveSample[Length];
			Channel[1] = new lWaveSample[Length];
		}
		else
		{
			Error |= WAV_ERR_UNSUPPORTED_FORMAT;
			return;
		}

        for(unsigned int i=0;i <Length;i++)
        {
            In.read((char *)Buffer,2);
            CombineShort_LittleEndian(Buffer,&Tmp);

            Channel[0][i] = (Tmp/32768.0);

            if(Channels == 2)
			{
				In.read((char *)Buffer,2);
				CombineShort_LittleEndian(Buffer,&Tmp);

				Channel[1][i] = (Tmp/32768.0);
			}
        }

        Error |= WAV_ERR_DATA_FOUND;
    }
}

lWaveSample lWave::GetSample_t(double t,unsigned int channel) const
{
	unsigned int cIndex = std::floor(t/GetPeriod());
	if(cIndex >= Length)
	{
		cIndex = cIndex % Length;
	}

	unsigned int nIndex = std::ceil(t/GetPeriod());
	if(nIndex >= Length)
	{
		nIndex = nIndex % Length;
	}

	double w = (t - cIndex*GetPeriod())/GetPeriod();

	lWaveSample Sample;
	if((Channels == 1 && channel == 0) || (Channels == 2 && channel < 2))
	{
		Sample = Channel[channel][cIndex] * w + Channel[channel][nIndex] * (1.0 - w);
	}
	else
	{
		Sample = 0.0;
	}

	return Sample;
}

void lWave::PrintError(std::ostream &out) const
{
    if(Error & WAV_ERR_COULDNT_OPEN_FILE)
    {
        out << "Error! Couldn't open file!\n";
    }
    if(Error & WAV_ERR_FILE_NOT_RECOGNIZED)
    {
        out << "Error! Opened file is not wav!\n";
    }
    if(! (Error & WAV_ERR_FMT_FOUND))
    {
        out << "Error! Fmt Header not found!\n";
    }
    if(! (Error & WAV_ERR_DATA_FOUND))
    {
        out << "Error! Data Header not found!\n";
    }
    if(Error & WAV_ERR_UNSUPPORTED_FORMAT)
    {
        out << "Error! Opened wav file has unsupported format!\n";
    }
}

lWave::lWave(const string &filename)
    :Channels(0),Frequency(0),BitsPerSample(0),/*Endian(WAVE_LITTLE_ENDIAN),*/Length(0),Error(0)
{
	Channel[0] = nullptr;
	Channel[1] = nullptr;

    ifstream In;
    In.open(filename.c_str(),ifstream::binary);

    if(!In.is_open())
    {
        cerr << "Couldn't open file " << filename << endl;
        Error |= WAV_ERR_COULDNT_OPEN_FILE;
        return;
    }

    unsigned char Buffer[12];
    In.read((char *)Buffer,12);

    if((Buffer[0] != 'R')||(Buffer[1] != 'I')||(Buffer[2] != 'F')||(Buffer[3] != 'F'))
	{
	    cerr << "File " << filename << " is not a RIFF file" << endl;
		Error |= WAV_ERR_FILE_NOT_RECOGNIZED;
		return;
	}

	int Length;
	CombineInt_LittleEndian(&Buffer[4],&Length);

	//fread(Buffer,sizeof(char),4,File);
	if((Buffer[8] != 'W')||(Buffer[9] != 'A')||(Buffer[10] != 'V')||(Buffer[11] != 'E'))
	{
	    cerr << "File " << filename << " is not a WAV file" << endl;
		Error |= WAV_ERR_FILE_NOT_RECOGNIZED;
		return;
	}

	int i=0;
	while(i < Length)
    {
        In.read((char *)Buffer,4);
        i += 4;

        if((Buffer[0] == 'f')&&(Buffer[1] == 'm')&&(Buffer[2] == 't')&&(Buffer[3] == ' '))
		{
			LoadFmt(In);

			if(!(Error & WAV_ERR_FMT_FOUND))
            {
                cerr << "Fmt chunk not found in file " << filename << endl;
                return;
            }

            i += 16 + 4;

			#ifdef WAVE_TESTING

			printf("Fmt chunk loaded\n");

			#endif
		}
		else if((Buffer[0] == 'd')&&(Buffer[1] == 'a')&&(Buffer[2] == 't')&&(Buffer[3] == 'a'))
		{
			LoadData(In);

			i += Length + 4;

			if(!(Error & WAV_ERR_DATA_FOUND))
            {
                cerr << "Data chunk not found in file " << filename << endl;
            }

			#ifdef WAVE_TESTING

			printf("Data chunk loaded\n");

			#endif
		}

    }
}

lWave::~lWave()
{
    delete Channel[0];
    delete Channel[1];
}

std::ostream &operator<<(std::ostream &out,const lWave &wave)
{
    out << "Channels: "<< wave.GetChannels() << ';'
        << "Frequency: "<< wave.GetFrequency() << ';'
        << "BitsPerSample: "<< wave.GetBitsPerSample() << ';'
        << "Length: "<< wave.GetLength() << ';';

	/*
    out << "\nData:\n";

    for(unsigned int i=0;i < wave.GetLength();i++)
    {
        out << wave[i] << ';';
    }
	*/
    return out;
}
