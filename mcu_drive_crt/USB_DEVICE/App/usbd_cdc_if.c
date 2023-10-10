/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */
#include <stdarg.h>
#include "commu.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t USB_RX_BUF[USB_REC_LEN];
uint8_t USB_RX_BUF1[USB_REC_LEN];//接收缓冲,最大USB_REC_LEN个字节.
uint8_t USB_RX_BUF2[USB_REC_LEN];//接收缓冲,最大USB_REC_LEN个字节.

uint16_t USB_RX_STA = 0;//接收状态标记（接收到的有效字节数量）
//uint8_t BMP_BUF[673]= {

//0x00,0x00,0x00,0x1F,0xFF,0x80,0x00,0x00,0x00,0x3F,0xFF,0x80,0x00,0x00,0x00,0x7F,
//0xFF,0x80,0x00,0x00,0x00,0xFF,0xFF,0xC0,0x00,0x00,0x00,0xFF,0xFF,0xC0,0x00,0x00,
//0x00,0xFF,0xFF,0xC0,0x00,0x00,0x00,0xFF,0xFF,0xE0,0x00,0x00,0x00,0xFF,0xFF,0xE0,
//0x00,0x00,0x00,0x7F,0xFF,0xE0,0x00,0x00,0x00,0x3F,0xFF,0xE0,0x00,0x00,0x00,0x3F,
//0xFF,0xF0,0x00,0x00,0x00,0x1F,0xFF,0xF0,0x00,0x00,0x00,0x1F,0xFF,0xF0,0x00,0x00,
//0x00,0x0F,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xFF,0xF8,
//0x00,0x00,0x00,0x1F,0xFF,0xF8,0x00,0x01,0x00,0x1F,0xFF,0xF8,0x00,0x07,0xE0,0x3F,
//0xFF,0xF0,0x00,0x0F,0xE0,0x3F,0xFF,0xC0,0x00,0x0F,0xF0,0x3F,0xFF,0xC0,0x00,0x0F,
//0xFF,0x7F,0xFE,0x00,0x00,0x0F,0xFF,0xFF,0xFE,0xE0,0x00,0x0F,0xFF,0xFF,0xFF,0xE0,/*"E:\科技资料\0224.bmp",0*/
//	
//};
uint32_t rx_len=0;
uint8_t rx_flag = 0,number=0;
uint8_t buf = 1; //记录当前使用的是buf1 还是 buf2
uint8_t disp = 0; //告诉main该显示什么内容，0不显示，1显示buf1，2显示buf2 
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */



/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */

static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */

	if(*Len<USB_REC_LEN)//判断收到数据量是否小于寄存器上限
	{
//		uint16_t i;
//	    USB_RX_STA = *Len;//将数据量值放入标志位
//	    for(i=0;i<*Len;i++)//循环
//		{
//			USB_RX_BUF[i] = Buf[i];//将数据内容放入数据寄存器
//		    rx_flag = 1;
////			if(Buf[number] == '#')
////			{
////				BMP_BUF[number++] = Buf[i];
////				rx_flag = 1;
////				break;
////			}//frame end flag
////			else
////			{
////				BMP_BUF[number++] = Buf[i];
////			}
//			if(buf == 1) //若果当前是使用的是buf1
//			{
//				buf = 2; //下次使用buf2
//				disp = 1;//告诉main显示buf1的内容到屏幕
//				memcpy(USB_RX_BUF2, Buf, 4800);
//			
//			}
//			else
//			{
//				buf = 1; //下次使用buf1
//				disp = 2;//告诉main显示buf2的内容到屏幕
//				memcpy(USB_RX_BUF1, Buf, 4800);
//			} 
//		
//		}
			__disable_irq();
			if(number < 24)
			{
				for( uint32_t i = 0; i < *Len; i ++ ){
					USB_RX_BUF[rx_len+i] = Buf[i];
				}
				rx_len += *Len;
				number++;
			}
			
			else
			{
				number = 0;
				rx_len = 0;
				if(buf == 1) //若果当前是使用的是buf1
				{
					buf = 2; //下次使用buf2
					disp = 1;//告诉main显示buf1的内容到屏幕
					memcpy(USB_RX_BUF2, USB_RX_BUF, 1600);
				
				}
				else
				{
					buf = 1; //下次使用buf1
					disp = 2;//告诉main显示buf2的内容到屏幕
					memcpy(USB_RX_BUF1, USB_RX_BUF, 1600);
				} 
//				memset(USB_RX_BUF, 0x00, 1280);
			}
			__enable_irq();
	}
	
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  uint32_t TimeStart = HAL_GetTick();
  USBD_CDC_HandleTypeDef *hcdc =  (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if(hcdc == 0) return  USBD_FAIL;  //避免未插入USB设备，出现死机的情况。
  //if (hcdc->TxState != 0) return  USBD_BUSY;
  while(hcdc->TxState)
  {
     if(HAL_GetTick()-TimeStart > 10)
    	 return USBD_BUSY;
     else
    	 break;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf,  Len);
  result =  USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  TimeStart = HAL_GetTick();
  while(hcdc->TxState)
  {
     if(HAL_GetTick()-TimeStart > 10)
	 return USBD_BUSY;
  }

  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
void USB_printf(const char *format, ...)//USB模拟串口的打印函数
{
    va_list args;
    uint32_t length;
    va_start(args, format);
    length = vsnprintf((char  *)UserTxBufferFS, APP_TX_DATA_SIZE, (char  *)format, args);
    va_end(args);
    CDC_Transmit_FS(UserTxBufferFS, length);
}


/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
