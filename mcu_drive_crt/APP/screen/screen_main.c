/*<FH+>************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*File name: 	  screen_main.c                                               */
/*Content summary:	显示应用源文件		*/
/*Other instructions:			*/
/*Current version:				*/
/*Author: 				*/
/*Completion date: 		*/
/*Modification record:	*/
/*Modification date       */
/* ------------------------------------------------------------------------------- */
/*    2023-09-26                V1.00        wangwentao             Create          */
/*<FH->************************************************************************/
#include "screen_main.h"
#include "ntsc.h"
#include "commu.h"
#include "../../USB_DEVICE/App/usbd_cdc_if.h"
//#define SCREEN_SIZE 1512
#define SCREEN_SIZE 3024
uint8_t SHOW_TMP_BUFF[SCREEN_SIZE];
void USB_Reset(void);


//uint8_t bitmap[] = {
//    0xbf, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xf0, 0x7d, 0xf3,
//    0xff, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x18, 0x73, 0xc7, 0xc1, 0xeb, 0xff, 0x6f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0xe8, 0x10, 0xe0, 0x0f, 0xe0,
//    0xe4, 0xff, 0xef, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x09, 0x00, 0xe0, 0xff, 0xff, 0xe9, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0x02, 0x00, 0xc0, 0xff,
//    0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0xec, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xbf, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff,
//    0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xe7, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xed, 0xfe, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xa1, 0xff, 0xff, 0xff,
//    0xff, 0xff, 0x9f, 0x87, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xf3, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff,
//    0x05, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x06, 0x00,
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xfe, 0xfe, 0xff, 0xff, 0xf7, 0xbf, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0xcf, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
//    0x00, 0x01, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xcf, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//    0x48, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x7f, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0xe8, 0xff, 0xff, 0xff, 0x7f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x20,
//    0xf8, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x01, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe6, 0xff, 0xff,
//    0xff, 0xff, 0xbf, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x1f, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x7c, 0x00, 0xfe, 0xff, 0xff,
//    0x1f, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x10, 0x00, 0xfc, 0xff, 0xff, 0x3f, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0xf8, 0xff, 0xff, 0x07, 0x00,
//    0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x0b, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xdf, 0x03, 0x00, 0x00, 0xe9,
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x80, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x00, 0x00, 0x80, 0xef, 0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0x00, 0x80, 0xff, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x9f, 0x00, 0x00, 0xc0, 0xff, 0xf1, 0x07, 0x00, 0x00, 0x00, 0x00,
//    0x00, 0x00, 0xe0, 0xff, 0x0f, 0x00, 0x40, 0xc0, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x0f, 0x00, 0x80, 0xd8, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//    0xf0, 0xff, 0x0f, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x0f, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff,
//    0x0f, 0x00, 0x00, 0xff, 0xff, 0xff, 0x1f, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x3f, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x1f, 0x00,
//    0xc0, 0xff, 0xff, 0xff, 0x7f, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f, 0x00, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x3f, 0x00, 0x40, 0xff,
//    0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x7f, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x7f, 0x00, 0xfc, 0xff, 0xff, 0xff,
//    0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x01, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x17, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x63, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff,
//    0x1b, 0x00, 0x00, 0x00, 0x60, 0xff, 0xff, 0xff, 0xef, 0x4c, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xdf, 0x89, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
//    0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xbf, 0x81, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x80, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x40, 0xfe, 0xbf, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00,
//    0xfc, 0xff, 0xff, 0xff, 0xdf, 0x05, 0xd4, 0xbf, 0xff, 0xff, 0xff, 0xff, 0x50, 0x00, 0xf0, 0xde, 0xff, 0xff, 0xff, 0xff, 0x01, 0x88, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xed, 0x81, 0xf8, 0xff, 0xff,
//    0xff, 0xff, 0xff, 0x01, 0xcc, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0xd8, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xd8, 0xfd, 0xff, 0xff, 0xff, 0xf7, 0xff, 0x3f, 0xcf, 0xff, 0xff, 0xff, 0xff,
//    0xff, 0x07, 0xde, 0xfd, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xdf, 0xee, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xd8, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x5f, 0x8f, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07,
//    0x4c, 0xff, 0xff, 0xff, 0x11, 0x04, 0x00, 0x43, 0xff, 0xf7, 0xff, 0xf2, 0xff, 0xff, 0x07, 0x36, 0xfb, 0xff, 0xeb, 0xbb, 0xee, 0x00, 0x97, 0xff, 0xff, 0xff, 0xf2, 0xff, 0xff, 0x47, 0xd4, 0xff,
//    0xff, 0x3b, 0x00, 0x7f, 0xe0, 0x61, 0xff, 0xff, 0xff, 0xfa, 0xff, 0xff, 0x0f, 0xfe, 0xff, 0xff, 0xff, 0xbf, 0x1f, 0x00, 0x00, 0xff, 0xff, 0xff, 0xf2, 0xff, 0xff, 0x3f, 0x80, 0xff, 0xff, 0xff,
//    0xff, 0x3f, 0x00, 0x00, 0xff, 0xff, 0xf7, 0xf2, 0xff, 0xff, 0xff, 0xa1, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xf6, 0xff, 0xff, 0xff, 0xff,
//};




void Screen_TaskSetup(void)
{
	Ntsc_Init(SC_224x108, SHOW_TMP_BUFF);
	USB_Reset();
	memset(SHOW_TMP_BUFF, 0x00, SCREEN_SIZE);


	memset(SHOW_TMP_BUFF, 0x00, SCREEN_SIZE);
	
	
	
}
void Screen_TaskLoop(void)
{
	
	Ntsc_Adjust(40, 0, 30);
	ShowText(2, 0, "STM32_CRT");
//	Com_Handle();
	if(disp == 1) 
	{
		disp = 0;
		DrawBMP(0,16,20,96,(unsigned char*)USB_RX_BUF1);
	;
	}
	else if(disp == 2)
	{
		disp = 0;
		DrawBMP(0,16,20,96,(unsigned char*)USB_RX_BUF2);
		
	}

}
/*USB 重新枚举函数
 * 解决问题：每次下载完程序后都要重新拔插一次USB PC才能识别串口，这是由于芯片在下载完程序后没有重新枚举所导致的。需要在MCU上电后对USB进行关闭，PC才能重新枚举识别。
 * 解决方法为将USB DP（PA12）引脚拉低一段时间后即可*/
void USB_Reset(void)
{
  	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
}

void USB_COM(void)
{
	  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//USB模拟串口的查寻接收处理
//		if(USB_RX_STA!=0)//判断是否有
//		{
//			//USB_printf("USB_RX:");//向USB模拟串口发字符串
//			CDC_Transmit_FS(USB_RX_BUF, USB_RX_STA);//USB串口：将接收的数据发回给电脑端
//			//USB_printf("\r\n");//向USB模拟串口发（回车）
//			USB_RX_STA=0;//数据标志位清0
//			memset(USB_RX_BUF,0, sizeof(USB_RX_BUF));//USB串口数据寄存器清0
//		}
  }
  /* USER CODE END 3 */

}

