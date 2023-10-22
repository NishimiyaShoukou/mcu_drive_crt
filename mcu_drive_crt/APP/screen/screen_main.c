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
#define SCREEN_SIZE 3024  //224*108/8
uint8_t SHOW_TMP_BUFF[SCREEN_SIZE];
void USB_Reset(void);





void Screen_TaskSetup(void)
{
	Ntsc_Init(SC_224x108, SHOW_TMP_BUFF);
	USB_Reset();
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

