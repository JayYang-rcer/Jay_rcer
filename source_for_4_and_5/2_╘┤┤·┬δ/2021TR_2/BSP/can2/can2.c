#include "headfiles.h"

// CAN2初始化 用于M2508与传感器
// PB12 -> CAN2RX 
// PB13 -> CAN2TX 
void can2_CAN2_init(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  // 使用CAN2时必须开启CAN1时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &gpio);

    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    CAN_DeInit(CAN2);
    CAN_StructInit(&can);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;    
    can.CAN_AWUM = DISABLE;    
    can.CAN_NART = DISABLE;    
    can.CAN_RFLM = DISABLE;    
    can.CAN_TXFP = ENABLE;     
    can.CAN_Mode = CAN_Mode_Normal; 
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;  // 比特率：1M
    CAN_Init(CAN2, &can);
    
    can_filter.CAN_FilterNumber = 14;
    can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh = 0x0000;
    can_filter.CAN_FilterIdLow = 0x0000;
    can_filter.CAN_FilterMaskIdHigh = 0x0000;
    can_filter.CAN_FilterMaskIdLow = 0x0000;
    can_filter.CAN_FilterFIFOAssignment = 0; 
    can_filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN2, CAN_IT_TME, ENABLE);
}


// CAN2发送中断
void CAN2_TX_IRQHandler(void)
{
  if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET)
  {
	   CAN_ClearITPendingBit(CAN2, CAN_IT_TME);   
  }
}

/*WAITING_TEST*/
// CAN2接收中断
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg CAN2_RX0_message;

	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
	{
		CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RX0_message);  
		
		AK80_update_info(&CAN2_RX0_message);
		
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
	}
}
