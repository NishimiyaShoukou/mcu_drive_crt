/*<FH+>************************************************************************/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*File name: 	  ntsc.c                                               */
/*Content summary:	显像管驱动源文件		*/
/*Other instructions:			*/
/*Current version:				*/
/*Author: 				*/
/*Completion date: 		*/
/*Modification record:	*/
/*Modification date       */
/* ------------------------------------------------------------------------------- */
/*    2023-11-26                V1.00        wangwentao             Create          */
/*<FH->************************************************************************/
#include "ntsc.h"
#include "stdlib.h"
#include "string.h"
#include "spi.h"
#include "tim.h"
#include "../../USB_DEVICE/App/usbd_cdc_if.h"
#include "front.h"

#define NTSC_S_TOP 3        // 垂直同步起始线
#define NTSC_S_END 5        // 垂直同期终止线
#define NTSC_VTOP 30        // 视频显示开始线


// 画面解像度別パラメタ設定
typedef struct  {
  uint16_t width;   // 画面水平分辨率
  uint16_t height;  // 画面垂直分辨率
  uint16_t ntscH;   // NTSC画面垂直数
  uint16_t hsize;   // 水平字节数
  uint8_t  flgHalf; // 是否半场显示 (0:通常 1:半分)
  uint32_t spiDiv;  // SPI分频大小，决定字符大小
} SCREEN_SETUP;

#define SPI_CLOCK_DIV2   SPI_BAUDRATEPRESCALER_2
#define SPI_CLOCK_DIV4   SPI_BAUDRATEPRESCALER_4
#define SPI_CLOCK_DIV8   SPI_BAUDRATEPRESCALER_8
#define SPI_CLOCK_DIV16  SPI_BAUDRATEPRESCALER_16
#define SPI_CLOCK_DIV32  SPI_BAUDRATEPRESCALER_32
#define SPI_CLOCK_DIV64  SPI_BAUDRATEPRESCALER_64
#define SPI_CLOCK_DIV128 SPI_BAUDRATEPRESCALER_128
#define SPI_CLOCK_DIV256 SPI_BAUDRATEPRESCALER_256
#define NTSC_TIMER_DIV 3 // 定时器分频周期
const SCREEN_SETUP screen_type[] = {
  { 112, 108, 216, 14, 1, SPI_CLOCK_DIV32 }, // 112x108
  { 224, 108, 216, 28, 1, SPI_CLOCK_DIV16 }, // 224x108 
  { 224, 216, 216, 28, 0, SPI_CLOCK_DIV16 }, // 224x216
  { 448, 108, 216, 56, 1, SPI_CLOCK_DIV8  }, // 448x108 
  { 448, 216, 216, 56, 0, SPI_CLOCK_DIV8  }, // 448x216 
};


#define NTSC_LINE (262+0)                     // 画面构成扫描线数（部分显示器对应用追加2条）

static uint8_t* vram;                         // 视频显示帧缓冲区参考指针
static volatile uint8_t* ptr;                 // 视频显示帧缓冲区参考指针
static volatile int count=1;                  // 数扫描线变量

static void (*_bktmStartHook)() = NULL;      
static void (*_bktmEndHook)()  = NULL;        
	
static uint8_t  _screen;
static uint16_t _width;
static uint16_t _height;
static uint16_t _ntscHeight;
static uint16_t _vram_size;
static uint16_t _ntsc_line = NTSC_LINE;
static uint16_t _ntsc_adjust = 0;
static uint16_t _hAdjust = 0; // 水平矫正
static uint16_t _vAdjust = 0; // 垂直矫正





uint16_t Ntsc_Width(void)  {return _width;} ;
uint16_t Ntsc_Height(void) {return _height;} ;
uint16_t Ntsc_Vram_Size(void) { return _vram_size;};
uint16_t Ntsc_Screen(void) { return _screen;};
uint8_t flgExtVram;

 // 消隐期开始挂钩设置
void NTSC_setBktmStartHook(void (*func)()) {
  _bktmStartHook = func;
}

// 消隐期结束挂钩设置
void NTSC_setBktmEndHook(void (*func)()) {
  _bktmEndHook = func;
}

// DMA中断清空输出
void NTSC_DMA1_CH3_handle() {
  // 等待SPI空闲
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);

// 向SPI数据寄存器发送0
	HAL_SPI_Transmit(&hspi1, (uint8_t*)0, 1, HAL_MAX_DELAY);
}

// DMA数据传输接口
void NTSC_SPI_DmaSend(uint8_t *transmitBuf, uint16_t length) {

	HAL_SPI_Transmit_DMA(&hspi1, transmitBuf, length);

}

/*<FUNC+>************************************************************************/
/* Function name:        DrawPointt                             */
/* Function Description:  显示点    */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void DrawPoint(int x, int y, uint8_t bit) {
  uint8_t* ptr = &Ntsc_Vram()[y*(Ntsc_Width()/8)+x];
  //CDC_Transmit_FS(ptr, 1); 
  *ptr = bit;

}


/*<FUNC+>************************************************************************/
/* Function name:        DrawFont                             */
/* Function Description:  显示字符    */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void ShowChar(int x, int y, uint8_t chr) {
	
	uint8_t i,j;
	//获取字符偏移量
	i = chr - ' ';
	if(i > 1 )
	{
		for (int j=0; j<16; j++) {
			
			DrawPoint(x, y + j, font8x16_Courier_New[i - 1][j]);

		}
	}
	else if(0 == i )
	{
		for (int j=0; j<16; j++) {
			
			DrawPoint(x, y + j, 0);

		}
	}
}

/*<FUNC+>************************************************************************/
/* Function name:        DrawText                             */
/* Function Description:  显示字符串    */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void ShowText(int x, int y, char* str) {
  
  while(*str != '\0') {
    if (x>=(Ntsc_Width()/8))
      break;

    ShowChar(x,y , *str);
    x++;
	str++;
  }  
}


void DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
	unsigned int j=0;
	unsigned char x,y;
  
//    if ((x1 - x0) >= (Ntsc_Width()/8))
//		return;
	for(y=y0;y<y1;y++)
	{
		
		for(x=x0;x<x1;x++)
		{
	    	 DrawPoint(x, y, BMP[j++]);	    	
	    }
	}
} 


/*<FUNC+>************************************************************************/
/* Function name:        Ntsc_HandleVout                             */
/* Function Description:   信号输出函数   */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void Ntsc_HandleVout(void) 
{

	if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC1) != RESET)
	{
		if (__HAL_TIM_GET_IT_SOURCE(&htim1, TIM_IT_CC1) != RESET)
		{
			/* Output compare event */		  
			__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC1);
			
			  if (count >= NTSC_VTOP + _vAdjust && count <= _ntscHeight+NTSC_VTOP + _vAdjust-1) 
			  {  	

					
				NTSC_SPI_DmaSend((uint8_t *)ptr, screen_type[_screen].hsize);
				if (screen_type[_screen].flgHalf) {
				  if ((count-NTSC_VTOP) & 1) 
				  ptr += screen_type[_screen].hsize;
				} else {
				  ptr += screen_type[_screen].hsize;
				}
				
				//CDC_Transmit_FS('c', 1);
			  }
				
			  // 设置同步脉冲大小
			  if(count >= NTSC_S_TOP-1 && count <= NTSC_S_END-1){
				// 垂直同步脉冲
				TIM1->CCR3 = 1412;
			  } else {
				// 水平同步脉冲
				TIM1->CCR3 = 112;
			  }

			  count++; 
			  if( count > _ntsc_line )
			  {
				count = 1;
				ptr = vram;    
			  } 

			
		  
		}
	}


}

/*<FUNC+>************************************************************************/
/* Function name:        Ntsc_Adjust                             */
/* Function Description:      */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void Ntsc_Adjust(int16_t cnt, int16_t hcnt, int16_t vcnt) 
{
  _ntsc_adjust = cnt;
  _ntsc_line = NTSC_LINE+cnt;
  _hAdjust = hcnt;
  _vAdjust = vcnt;
}
	
/*<FUNC+>************************************************************************/
/* Function name:        Ntsc_Init                             */
/* Function Description:   Ntsc_Init   */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void Ntsc_Init(uint8_t mode, uint8_t* extram)
{
	   // 设置屏幕参数
    _screen = mode <=4 ? mode: SC_DEFAULT;
	_width  = screen_type[_screen].width;
    _height = screen_type[_screen].height;   
	_vram_size  = screen_type[_screen].hsize * _height;
	_ntscHeight = screen_type[_screen].ntscH;

	flgExtVram = 0;

	if (extram) {
		vram = extram;
		flgExtVram = 1;
	} else {
		vram = (uint8_t*)malloc(_vram_size);  // 申请显存大小
	}

	Ntsc_Cls();
	ptr = vram;  //设置显示存储
	count = 1;

  // SPI初始化速度

    
	hspi1.Init.BaudRatePrescaler = screen_type[_screen].spiDiv;

	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}


  
  /// 定时器初始化
	HAL_TIM_Base_Stop(&htim1);

	
	TIM1->PSC = NTSC_TIMER_DIV - 1;
	TIM1->ARR = 1524;

  // +4.7us 水平同步信号

	TIM1->CCR3 = 112;
  
  // +9.4us 视频输出中断处理
	TIM1->CCR1 = 225-60 + _hAdjust;

	TIM1->CNT = 0;
	/* 允许中断 */
	TIM1->EGR |= 0X01;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);


	HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_OC_Start_IT(&htim1, TIM_CHANNEL_1);
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	HAL_TIM_Base_Start(&htim1);
 
	
	
}

/*<FUNC+>************************************************************************/
/* Function name:        Ntsc_End                             */
/* Function Description:   结束图像传输   */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void Ntsc_End(void) 
{
	HAL_TIM_Base_Stop(&htim1);
	HAL_TIM_Base_Stop_IT(&htim1);
	
	HAL_SPI_DMAStop(&hspi1);
	__HAL_DMA_DISABLE_IT(&hdma_spi1_tx, DMA_IT_TC);
	__HAL_SPI_DISABLE(&hspi1);
	if (!flgExtVram)
		free(vram);


}

/*<FUNC+>************************************************************************/
/* Function name:        Ntsc_Vram                             */
/* Function Description:   获取显存地址   */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
uint8_t* Ntsc_Vram(void) {
  return vram;  
}

/*<FUNC+>************************************************************************/
/* Function name:        Ntsc_Cls                             */
/* Function Description:   清空显示   */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void Ntsc_Cls(void) {
  memset(vram, _vram_size, 0);
}

/*<FUNC+>************************************************************************/
/* Function name:        NtscDelay_frame                             */
/* Function Description:   帧刷新等待   */
/* Input parameters:                                                            */
/* Output parameters: None                                                      */
/* Return value: None                                                           */
/* Operation process:                                                           */
/* Other notes: None                                                            */
/* Modification record:                                                         */
/*  Modification Date     Version No.     Modified By      Modification Content */
/* ---------------------------------------------------------------------------- */
/*     2023/12/22           V1.00          wangwentao            Create           */
/*<FUNC->************************************************************************/
void NtscDelay_frame(uint16_t x) {
  while (x) {
    while (count != _ntscHeight + NTSC_VTOP);
    while (count == _ntscHeight + NTSC_VTOP);
    x--;
  }
}

	


