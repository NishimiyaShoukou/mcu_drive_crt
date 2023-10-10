/*<FH+>************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*File name: 	 ntsc.h                                                */
/*Content summary:		显示应用头文件	*/
/*Other instructions:			*/
/*Current version:				*/
/*Author: 				*/
/*Completion date: 		*/
/*Modification record:	*/
/*Modification date       */
/* ------------------------------------------------------------------------------- */
/*    2023-11-26                V1.00        wangwentao             Create          */
/*<FH->************************************************************************/
#ifndef __SCREEN_MAIN_H__
#define __SCREEN_MAIN_H__

#include "main.h"


void Screen_TaskSetup(void);

void Screen_TaskLoop(void);

void USB_COM(void);
void USB_Reset(void);


#endif
