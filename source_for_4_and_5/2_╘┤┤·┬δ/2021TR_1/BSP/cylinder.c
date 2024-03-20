#include "includes.h"

void Cylinder_GPIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;		
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // 开时钟

	// 初始化相应的GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  // 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOF, GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	Cylinder_1_OFF();
	Cylinder_2_ON();
	Cylinder_3_OFF();
	Cylinder_4_OFF();
}


// 取箭
void Cylinder_1_ON(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
}
void Cylinder_1_OFF(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_10);
}
void Cylinder_2_ON(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_11);
}
void Cylinder_2_OFF(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_11);
}

// 吹箭
void Cylinder_3_ON(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_15);
}
void Cylinder_3_OFF(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_15);
}
void Cylinder_4_ON(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_14);
}
void Cylinder_4_OFF(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_14); 
}


