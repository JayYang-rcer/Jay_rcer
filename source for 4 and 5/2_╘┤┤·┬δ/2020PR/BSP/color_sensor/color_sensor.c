#include "includes.h"


// 色标传感器所用的GPIO初始化
// 竖直左侧色标 -> PA8
// 水平左侧色标 -> PC8
// 水平右侧色标 -> PC9
// 竖直右侧色标 -> PB8
// NPN型 触发接地
void cs_GPIO_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;		
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // 开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  // 开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  // 开时钟
	
	//初始化相应的GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  // 普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 上拉
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//初始化相应的GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  // 普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 上拉
	
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	//初始化相应的GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  // 普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // 上拉
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


// 获取色标传感器电平状态
// 低电平返回0(RESET)
// 高电平返回1(SET)
uint8_t Read_color(u8 sensor)
{
	uint8_t status = SET;  // 默认上拉
	
	switch(sensor)
	{
		case VERTICAL_LEFT_COLOR_SENSOR : status = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_0) ; break;
		case ACLINIC_LEFT_COLOR_SENSOR  : status = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) ; break;		
		case ACLINIC_RIGHT_COLOR_SENSOR : status = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) ; break;
		case VERTICAL_RIGHT_COLOR_SENSOR: status = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_1) ; break;
		default: break;
	}
	
	return status;
}

