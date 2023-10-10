/*<FH+>************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*File name: 	  commu.c                                               */
/*Content summary:	通信处理源文件		*/
/*Other instructions:			*/
/*Current version:				*/
/*Author: 				*/
/*Completion date: 		*/
/*Modification record:	*/
/*Modification date       */
/* ------------------------------------------------------------------------------- */
/*    2023-09-26                V1.00        wangwentao             Create          */
/*<FH->************************************************************************/
#include "commu.h"
#include "ntsc.h"
#include "../../USB_DEVICE/App/usbd_cdc_if.h"


/*<FUNC+>************************************************************************/
/* Function name:        Com_Handle                             */
/* Function Description:  USB虚拟串口交互函数    */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/09/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void Com_Handle(void)
{
	static uint8_t ucIndex;
	
	#if USB_MONITOR

	ShowText(2, 0, "STM32_CRT");
	
//	ShowText(0, (Ntsc_Height()) - 17, "left");
	ShowText(0, 16, ">");
	DrawPoint((Ntsc_Width()/8) - 2, (Ntsc_Height()) - 1, 0xff);
	DrawPoint(0, 0, 0xff);
	DrawPoint((Ntsc_Width()/8) - 2, 0, 0xff);
//	if(0 == rx_flag)
//	{
//		ShowText(2 + ucIndex, 16, "_");
//		HAL_Delay(200);
//		ShowText(2 + ucIndex, 16, " ");
//		HAL_Delay(200);	
//	}
//	else
//	{
//		
//		
//		ShowText(2 + ucIndex, 16, USB_RX_BUF);
//		
//		ucIndex += strlen(USB_RX_BUF);
//		
//		rx_flag = 0;
//		
//	}
	if(1 == rx_flag)
	{
		ShowChar(2 , 16, USB_RX_BUF1[0]);	
		rx_flag = 0;
	}
	
	#endif

	
	
}


