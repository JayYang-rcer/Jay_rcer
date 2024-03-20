#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define Max_BUFF_Len 				18	  

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         				//接收状态标记	

extern u8 Uart6_Buffer[Max_BUFF_Len];
extern u8 RES_2;

typedef struct ENCODER
{
	int angle_data1;
	int angle_data2;
	int angle_data3;	
	int angle_data4;	
	
	int check_data1;
	int check_data2;
	float angle;

}ENCODER;

#define ABSOLUTE_ENCODER 1024.0f
#define REDUCTION 18/66

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void USART2_Init(uint32_t BaudRate);
void UART5_Init(uint32_t BaudRate);
void Action_Uart_init(u32 baud_rate);
void USART6_init(u32 bound);
void Usart6_Send_Order(u8 order);
#endif


