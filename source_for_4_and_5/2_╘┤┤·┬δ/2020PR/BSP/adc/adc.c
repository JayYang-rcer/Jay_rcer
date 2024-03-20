#include "includes.h"

float RANGING_DISTANCE = 0;



// 初始化激光测距传感器用的ADC3
// PF9 -> ADC3CH7
void adc_adc3_init(void)
{
	// 声明结构体
  GPIO_InitTypeDef      GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	NVIC_InitTypeDef      NVIC_InitStructure;
	
	// 开时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  // 使能GPIOF时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  // 使能ADC3时钟

  //初始化GPIO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  // 模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  // 不带上下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);  // 初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);	  //ADC3复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);	 // 复位结束	 
 
  // 配置ADC3
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  // 独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;  // 预分频4分频，ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // DMA失能（独立模式无关）
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  // 两个采样阶段之间的延迟5个时钟（独立模式无关）
	ADC_CommonInit(&ADC_CommonInitStructure);  // 初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  // 12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // 非扫描模式（多通道才需要）
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // 连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  // 禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;  // 1个转换在规则序列中,也就是只转换规则序列1 
  ADC_Init(ADC3, &ADC_InitStructure);  // ADC初始化
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_480Cycles);  // 配置ADC3CH7第一个转换，采样时间为480个周期
	
	// 配置中断（最高优先级）
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);		

	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE); // 开启ADC3转换结束中断
	
	ADC_Cmd(ADC3, ENABLE);  // 使能ADC3
	
	ADC_SoftwareStartConv(ADC3);  // 开始ADC转换，软件触发
	

}


// ADC中断函数
void ADC_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC3, ADC_IT_EOC) == SET)
	{
		ADC_ClearITPendingBit(ADC3, ADC_IT_EOC);
	}
}





