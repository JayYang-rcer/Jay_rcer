#include "headfiles.h"


// 蜂鸣器所用GPIO初始化
// BEEP -> PF2
void Beep_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;		
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // 开时钟

	//初始化相应的GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	// 先关闭蜂鸣器
	GPIO_ResetBits(GPIOF, GPIO_Pin_2);
}



//蜂鸣器指示，1s
void Beep_ON_OFF(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_2);
	vTaskDelay(1000);
	GPIO_ResetBits(GPIOF, GPIO_Pin_2);
	vTaskDelay(1000);
}
