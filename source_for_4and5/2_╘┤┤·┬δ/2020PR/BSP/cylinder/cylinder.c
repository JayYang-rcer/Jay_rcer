#include "includes.h"



void cylinder_GPIO_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

		//初始化所有气缸的GPIO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOF, &GPIO_InitStructure);
		
		// 先关闭所有电磁阀
		GPIO_ResetBits(GPIOF, GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10);
}


// 开启气缸
void cylinder_on(u8 cylinder)
{
	switch(cylinder)
	{
		case HOLD_FOOT               : GPIO_SetBits(GPIOF, GPIO_Pin_15); break;
    case SHOOT_PULL              : GPIO_SetBits(GPIOF, GPIO_Pin_14); break;
    case CATCH_BALL_2ST          : GPIO_SetBits(GPIOF, GPIO_Pin_13); break;
    case TURN_CATCH              : GPIO_SetBits(GPIOF, GPIO_Pin_11); break; 
    case CATCH_BALL_1ST          : GPIO_SetBits(GPIOF, GPIO_Pin_12); break; 
		case CATCH_BALL_SEAT         : GPIO_SetBits(GPIOF, GPIO_Pin_10); break; 
		default: break;
	}
}


void cylinder_off(u8 cylinder)
{
	switch(cylinder)
	{
    case HOLD_FOOT               : GPIO_ResetBits(GPIOF, GPIO_Pin_15); break;
    case SHOOT_PULL              : GPIO_ResetBits(GPIOF, GPIO_Pin_14); break;
    case CATCH_BALL_2ST          : GPIO_ResetBits(GPIOF, GPIO_Pin_13); break;
    case TURN_CATCH              : GPIO_ResetBits(GPIOF, GPIO_Pin_11); break; 
    case CATCH_BALL_1ST          : GPIO_ResetBits(GPIOF, GPIO_Pin_12); break; 
		case CATCH_BALL_SEAT         : GPIO_ResetBits(GPIOF, GPIO_Pin_10); break; 
		default: break;
	}
}
