#include "includes.h"


struct SAngle 	GYRO_ANGLE;

/*
struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;
struct SQ       stcQ;
*/


// 配置陀螺仪使用的UART4
void gyro_UART4_init(u32 baud_rate)
{
	// 声明结构体
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);  // 使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);  // 使能USART4时钟
 
	// 串口4对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);  // PC10复用为USART4 TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);  // PC11复用为USART4 RX
	
	// UART4端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 // 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

  // UART4初始化设置
	USART_InitStructure.USART_BaudRate = baud_rate;                                  // 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                      // 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                           // 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                              // 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                 // 收发模式
  USART_Init(UART4, &USART_InitStructure);
	
  USART_Cmd(UART4, ENABLE);  // 使能串口4
	
	// 配置中断
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;           // 串口4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		     // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			       // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);  // 开启相关中断
}


// 串口4接受中断函数
void UART4_IRQHandler(void)
{
	static uint8_t UART4_receive_data;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)   
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);  // 清除标志位
		UART4_receive_data = USART_ReceiveData(UART4);  // 读取数据
		gyro_processing_gyro_data((unsigned char)UART4_receive_data);  // 处理数据
	}
}


// 处理串口4接受到的数据
void gyro_processing_gyro_data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++] = ucData;	 // 将收到的数据存入缓冲区中

	if(ucRxBuffer[0] != 0x55)  // 数据头不对，则重新开始寻找0x55数据头(数据头固定不变）
	{
		ucRxCnt = 0;
		return;
	}
	if(ucRxCnt < 11) {return;}  // 数据不满11个，则返回
	else
	{
		// 判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		switch(ucRxBuffer[1])
		{
			// memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析
			case 0x53:	memcpy(&GYRO_ANGLE, &ucRxBuffer[2], 8); break;
			/*
			case 0x50:	memcpy(&stcTime, &ucRxBuffer[2], 8); break;
			case 0x51:	memcpy(&stcAcc, &ucRxBuffer[2], 8); break;
			case 0x52:	memcpy(&stcGyro, &ucRxBuffer[2], 8); break;
			case 0x54:	memcpy(&stcMag, &ucRxBuffer[2], 8); break;
			case 0x55:	memcpy(&stcDStatus, &ucRxBuffer[2], 8); break;
			case 0x56:	memcpy(&stcPress, &ucRxBuffer[2], 8); break;
			case 0x57:	memcpy(&stcLonLat, &ucRxBuffer[2], 8); break;
			case 0x58:	memcpy(&stcGPSV, &ucRxBuffer[2], 8); break;
			case 0x59:	memcpy(&stcQ, &ucRxBuffer[2], 8); break;
			*/
		} 
		ucRxCnt = 0;  // 清空缓存区
	}
}

