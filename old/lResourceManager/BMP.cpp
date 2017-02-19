#include <iostream>
#include <fstream>

using namespace std;

#include "BMP.h"

int16_t Cmb_sInt16_LE(char *Src)
{
	signed long int Int = 0;
	Int |= ((unsigned char)Src[0] | ((unsigned char)Src[1] << 8));
	return Int;
}

uint16_t Cmb_uInt16_LE(char *Src)
{
	unsigned long int Int = 0;
	Int |= ((unsigned char)Src[0] | ((unsigned char)Src[1] << 8));
	return Int;
}

int32_t Cmb_sInt32_LE(char *Src)
{
	signed long int Int = 0;
	Int |= ((unsigned char)Src[0] | ((unsigned char)Src[1] << 8) | ((unsigned char)Src[2] << 16) | ((unsigned char)Src[3] << 24));
	return Int;
}

uint32_t Cmb_uInt32_LE(char *Src)
{
	unsigned long int Int = 0;
	Int |= ((unsigned char)Src[0] | ((unsigned char)Src[1] << 8) | ((unsigned char)Src[2] << 16) | ((unsigned char)Src[3] << 24));
	return Int;
}










const uint32_t BMP::RED_ARRAY  []          = {  0x00F80000,/*PF_B5_G6_R5*/
                                                0x1F000000,/*PF_R5_G6_B5*/
                                                0x000F0000,/*PF_A4_B4_G4_R4*/
                                                0x0F000000,/*PF_A4_R4_G4_B4*/
                                                0x007C0000,/*PF_B5_G5_R5_A1*/
                                                0x007C0000,/*PF_B5_G5_R5_X1*/
                                                0x1F000000,/*PF_R5_G5_B5_A1*/
                                                0x1F000000,/*PF_R5_G5_B5_X1*/
                                                0x000003FF,/*PF_A2_B10_G10_R10*/
                                                0x3FF00000,/*PF_A2_R10_G10_B10*/
                                                0x000000FF,/*PF_A8_B8_G8_R8*/
                                                0x000000FF,/*PF_X8_B8_G8_R8*/
                                                0x00FF0000,/*PF_A8_R8_G8_B8*/
                                                0x00FF0000,/*PF_X8_R8_G8_B8*/
                                             };

const uint32_t BMP::GREEN_ARRAY[]          = {  0xE0070000,/*PF_B5_G6_R5*/
                                                0xE0070000,/*PF_R5_G6_B5*/
                                                0x00F00000,/*PF_A4_B4_G4_R4*/
                                                0x00F00000,/*PF_A4_R4_G4_B4*/
                                                0xE0030000,/*PF_B5_G5_R5_A1*/ /*0x03E0*/
                                                0xE0030000,/*PF_B5_G5_R5_X1*/
                                                0xE0030000,/*PF_R5_G5_B5_A1*/
                                                0xE0030000,/*PF_R5_G5_B5_X1*/
                                                0x000FFC00,/*PF_A2_B10_G10_R10*/
                                                0x000FFC00,/*PF_A2_R10_G10_B10*/
                                                0x0000FF00,/*PF_A8_B8_G8_R8*/
                                                0x0000FF00,/*PF_X8_B8_G8_R8*/
                                                0x0000FF00,/*PF_A8_R8_G8_B8*/
                                                0x0000FF00,/*PF_X8_R8_G8_B8*/
                                             };

const uint32_t BMP::BLUE_ARRAY []          = {  0x1F000000,/*PF_B5_G6_R5*/
                                                0x00F80000,/*PF_R5_G6_B5*/
                                                0x0F000000,/*PF_A4_B4_G4_R4*/
                                                0x000F0000,/*PF_A4_R4_G4_B4*/
                                                0x1F000000,/*PF_B5_G5_R5_A1*/
                                                0x1F000000,/*PF_B5_G5_R5_X1*/
                                                0x007C0000,/*PF_R5_G5_B5_A1*/
                                                0x007C0000,/*PF_R5_G5_B5_X1*/
                                                0x3FF00000,/*PF_A2_B10_G10_R10*/
                                                0x000003FF,/*PF_A2_R10_G10_B10*/
                                                0x00FF0000,/*PF_A8_B8_G8_R8*/
                                                0x00FF0000,/*PF_X8_B8_G8_R8*/
                                                0x000000FF,/*PF_A8_R8_G8_B8*/
                                                0x000000FF,/*PF_X8_R8_G8_B8*/
                                             };

const uint32_t BMP::ALPHA_ARRAY[]          = {  0x00000000,/*PF_B5_G6_R5*/
                                                0x00000000,/*PF_R5_G6_B5*/
                                                0xF0000000,/*PF_A4_B4_G4_R4*/
                                                0xF0000000,/*PF_A4_R4_G4_B4*/
                                                0x00800000,/*PF_B5_G5_R5_A1*/
                                                0x00000000,/*PF_B5_G5_R5_X1*/
                                                0x00800000,/*PF_R5_G5_B5_A1*/
                                                0x00000000,/*PF_R5_G5_B5_X1*/
                                                0xC0000000,/*PF_A2_B10_G10_R10*/
                                                0xC0000000,/*PF_A2_R10_G10_B10*/
                                                0xFF000000,/*PF_A8_B8_G8_R8*/
                                                0x00000000,/*PF_X8_B8_G8_R8*/
                                                0xFF000000,/*PF_A8_R8_G8_B8*/
                                                0x00000000,/*PF_X8_R8_G8_B8*/
                                             };
