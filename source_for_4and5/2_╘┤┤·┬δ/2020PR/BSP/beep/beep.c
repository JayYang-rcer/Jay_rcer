#include "includes.h"


u8 BEEP_FLAG = 0;

// 记录蜂鸣器开始开启时TIM2寄存器的值
u32 BEEP_TURN_ON_TIMESTAMP = 0;


// 蜂鸣器所用GPIO初始化
// BEEP -> PF8
void beep_GPIO_init(void)
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
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}






