#ifndef INT
#define INT

void CombineByte(unsigned char *Source,void *Dest);
void CombineShort_LittleEndian(unsigned char *Source,void *Dest);
void CombineInt_LittleEndian(unsigned char *Source,void *Dest);

signed short Short_ClampInt(int x);

#endif
