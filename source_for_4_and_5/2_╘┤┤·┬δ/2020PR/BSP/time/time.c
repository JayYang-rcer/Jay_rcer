#include "includes.h"


// 配置编码器使用的TIM4
// PD12 -> A相  
// PD13 -> B相
void encoder_TIM4_init(void)
{
	// 声明结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  
	// 开时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
  // 配置GPIO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;  // PD12 -> A相  PD13 -> B相
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  // 复用模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  // 开漏输出模式 输入模式下不关心
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  // 上拉输入模式
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// 配置复用
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
  
  // 配置定时器
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  // 初始化结构体参数值
	
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 无预分频值 不关心
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  // 预装载值设置为最大
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频因子 不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式   
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  // 配置定时器的编码器模式 直接映射到TI1 TI2 都不反相
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;  // 输入滤波器配置为0
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
 
	// 开启定时器
  TIM_Cmd(TIM4, ENABLE);  // 使能定时器
}


// 配置编码器使用的TIM8
// PC6 -> A相  
// PC7 -> B相
void encoder_TIM8_init(void)
{
	// 声明结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  
	// 开时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
  // 配置GPIO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  // PC6 -> A相  PC7 -> B相
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  // 复用模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  // 开漏输出模式 输入模式下不关心
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  // 上拉输入模式
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	// 配置复用
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
  
  // 配置定时器
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  // 初始化结构体参数值
	
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 无预分频值 不关心
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  // 预装载值设置为最大
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频因子 不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 向上计数模式   
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
  
  // 配置定时器的编码器模式 直接映射到TI1 TI2 都不反相
	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;  // 输入滤波器配置为0
  TIM_ICInit(TIM8, &TIM_ICInitStructure);
 
	// 开启定时器
  TIM_Cmd(TIM8, ENABLE);  // 使能定时器
}

void Launch_Motor_init(u32 arr,u32 psc)//use TIM12 and PB14-15 as the PWM channel of Launch_motor
{					 
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12); 

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse = 1000;
	TIM_OC1Init(TIM12, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  
  TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable); 
  TIM_ARRPreloadConfig(TIM12,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM12, ENABLE); 
 
}


void TIM9_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  	//TIM9时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9); //GPIOE5复用为定时器9
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_TIM9); //GPIOE6复用为定时器9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM9,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM9, ENABLE);  //使能TIM14
 
										  
}  


// 获取定时器CNT寄存器里面的值 用于下一步位置信息的计算
u32 encoder_get_encoder_CNT(u8 TIMx)
{
	 u32 encoder_CNT;    
   switch(TIMx)
	 {	   
		 case 4: encoder_CNT = (short)TIM4 -> CNT; TIM4 -> CNT = 0; break;	// 读取后清零
		 case 8: encoder_CNT = (short)TIM8 -> CNT; TIM8 -> CNT = 0; break;	// 读取后清零
		 default: encoder_CNT = 0;
	 }
	return encoder_CNT;
}





