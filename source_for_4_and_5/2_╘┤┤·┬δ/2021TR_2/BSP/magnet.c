#include "headfiles.h"


// 磁铁初始化
void Magnet_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;		
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // 开时钟

	//初始化相应的GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}


void Throw_Enable(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}
void Throw_Disable(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}
