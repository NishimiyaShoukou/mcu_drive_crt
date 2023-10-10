/*<FH+>************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*File name: 	 ntsc.h                                                */
/*Content summary:		ntscÍ·ÎÄ¼þ	*/
/*Other instructions:			*/
/*Current version:				*/
/*Author: 				*/
/*Completion date: 		*/
/*Modification record:	*/
/*Modification date       */
/* ------------------------------------------------------------------------------- */
/*    2023-11-26                V1.00        wangwentao             Create          */
/*<FH->************************************************************************/
#ifndef __NTSC_H__
#define __NTSC_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
	
#define SC_112x108  0 // 112x108
#define SC_224x108  1 // 224x108
#define SC_224x216  2 // 224x216
#define SC_448x108  3 // 448x108
#define SC_448x216  4 // 448x216
#define SC_DEFAULT  SC_224x216


void Ntsc_Init(uint8_t mode, uint8_t* extram);

void Ntsc_End(void);

void Ntsc_Adjust(int16_t cnt, int16_t hcnt, int16_t vcnt);

uint8_t* Ntsc_Vram(void);

void Ntsc_HandleVout(void);

void Ntsc_Cls(void);

void NtscDelay_frame(uint16_t x);

uint16_t Ntsc_Width(void);

uint16_t Ntsc_Height(void);

uint16_t Ntsc_Vram_Size(void);

uint16_t Ntsc_Screen(void);

void DrawPoint(int x, int y, uint8_t bit);


void ShowChar(int x, int y, uint8_t chr);
void ShowText(int x, int y, char* str);
void drawFont(int x, int y, uint8_t* font);
void DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);


#ifdef __cplusplus
}
#endif


#endif
