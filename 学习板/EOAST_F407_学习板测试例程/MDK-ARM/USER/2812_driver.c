#include "2812_driver.h"
#include "tim.h"
#include "math.h"
/*Some Static Colors------------------------------*/
const RGB_Color_TypeDef RED      = {255,0,0};   //显示红色RGB数据
const RGB_Color_TypeDef GREEN    = {0,255,0};
const RGB_Color_TypeDef BLUE     = {0,0,255};
const RGB_Color_TypeDef SKY      = {0,255,255};
const RGB_Color_TypeDef MAGENTA  = {255,0,220};
const RGB_Color_TypeDef YELLOW   = {127,216,0};
const RGB_Color_TypeDef OEANGE   = {127,106,0};
const RGB_Color_TypeDef BLACK    = {0,0,0};
const RGB_Color_TypeDef WHITE    = {255,255,255};
 
/*二维数组存放最终PWM输出数组，每一行24个
数据代表一个LED，最后一行24个0代表RESET码*/
uint32_t Pixel_Buf[Pixel_NUM+1][24];       
 
/*
功能：设定单个RGB LED的颜色，把结构体中RGB的24BIT转换为0码和1码
参数：LedId为LED序号，Color：定义的颜色结构体
*/
//延时函数
 void delay_ms(int32_t nms) 
 {
  int32_t temp; 
  SysTick->LOAD = 10000*nms; 
  SysTick->VAL=0X00;//清空计数器 
  SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源 
  do 
  { 
       temp=SysTick->CTRL;//读取当前倒计数值 
		
  } while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达 
   SysTick->CTRL=0x00; //关闭计数器 
   SysTick->VAL =0X00; //清空计数器 
 }
void RGB_SetColor(uint8_t LedId,RGB_Color_TypeDef Color)
{
	uint8_t i; 
	if(LedId > Pixel_NUM)return; //avoid overflow 防止写入ID大于LED总数
	
	for(i=0;i<8;i++) Pixel_Buf[LedId][i]   = ( (Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
	for(i=8;i<16;i++) Pixel_Buf[LedId][i]  = ( (Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
	for(i=16;i<24;i++) Pixel_Buf[LedId][i] = ( (Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
}
 
/*
功能：最后一行装在24个0，输出24个周期占空比为0的PWM波，作为最后reset延时，这里总时长为24*1.2=30us > 24us(要求大于24us)
*/
void Reset_Load(void)
{
	uint8_t i;
	for(i=0;i<24;i++)
	{
		Pixel_Buf[Pixel_NUM][i] = 0;
	}
}
 
/*
功能：发送数组
参数：(&htim1)定时器1，(TIM_CHANNEL_1)通道1，((uint32_t *)Pixel_Buf)待发送数组，
			(Pixel_NUM+1)*24)发送个数，数组行列相乘
*/
void RGB_SendArray(void)
{
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)Pixel_Buf,(Pixel_NUM+1)*24);
}
 
/*
功能：显示红色
参数：Pixel_Len为显示LED个数
*/
void RGB_RED(uint16_t Pixel_Len)
{
	uint16_t i;
	for(i=0;i<Pixel_Len;i++)//给对应个数LED写入红色
	{
		RGB_SetColor(i,RED);
	}
	Reset_Load();
	RGB_SendArray();
}
 
/*
功能：显示绿色
参数：Pixel_Len为显示LED个数
*/
void RGB_GREEN(uint16_t Pixel_Len)
{
	uint16_t i;
	for(i=0;i<Pixel_Len;i++)//给对应个数LED写入绿色
	{
		RGB_SetColor(i,GREEN);
	}
	Reset_Load();
	RGB_SendArray();
}
 
/*
功能：显示蓝色
参数：Pixel_Len为显示LED个数
*/
void RGB_BLUE(uint16_t Pixel_Len)
{
	uint16_t i;
	for(i=0;i<Pixel_Len;i++)//给对应个数LED写入蓝色
	{
		RGB_SetColor(i,BLUE);
	}
	Reset_Load();
	RGB_SendArray();
}
 
/*
功能：显示白色
参数：Pixel_Len为显示LED个数
*/
void RGB_WHITE(uint16_t Pixel_Len)
{
	uint16_t i;
	for(i=0;i<Pixel_Len;i++)//给对应个数LED写入白色
	{
		RGB_SetColor(i,WHITE);
	}
	Reset_Load();
	RGB_SendArray();
}

void RGB_BLACK(uint16_t Pixel_Len)
{
	uint16_t i;
	for(i=0;i<Pixel_Len;i++)//给对应个数LED写入黑色
	{
		RGB_SetColor(i,BLACK);
	}
	Reset_Load();
	RGB_SendArray();
}

void flicker_RGB(uint16_t Pixel_Len,uint16_t color,int number)
{
	int i=0,j=1;
	if(j==1)
	{
	for(i=0;i<number;i++)
	{
		uint32_t starttime=HAL_GetTick();
		if(color==1)
		{
		RGB_RED(Pixel_Len);
    delay_ms(700);
		RGB_BLACK(Pixel_Len );
		delay_ms(150);
		}
		if(color==2)
		{
		RGB_GREEN(Pixel_Len);
    delay_ms(700);
		RGB_BLACK(Pixel_Len );
		delay_ms(150);
 
		}
		
	  if(color==3)
		{
		RGB_WHITE(Pixel_Len);
    delay_ms(700);
		RGB_BLACK(Pixel_Len );
		delay_ms(150);
		}	
	}
	j=2;
  }
	if(j==2)
	{
		for(i=0;i<6;i++)
	{
		delay_ms(1000);
	}
		j=1;
	}
	
}
//也可以继续添加其他颜色，和颜色变化函数等





