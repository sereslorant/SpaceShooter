#include "Int.h"

void CombineByte(unsigned char *Source,void *Dest)
{
	*(char*)Dest = 0x00;
	*(char*)Dest |= Source[0];
}
void CombineShort_LittleEndian(unsigned char *Source,void *Dest)
{
	*(short*)Dest = 0x0000;
	*(short*)Dest |= (Source[0] | (Source[1] << 8));
}

void CombineInt_LittleEndian(unsigned char *Source,void *Dest)
{
	*(int*)Dest = 0x00000000;
	*(int*)Dest |= (Source[0] | (Source[1] << 8) | (Source[2] << 16) | (Source[3] << 24));
}

signed short Short_ClampInt(int x)
{
    if(x > 32767)
    {
        return 32767;
    }
    if(x < -32768)
    {
        return 32768;
    }

    return x;
}
