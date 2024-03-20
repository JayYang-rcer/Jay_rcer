#ifndef __TX_HANDLE_H
#define __TX_HANDLE_H

#define USART_TX	USART2
#define USART_RX	UART5

#define START   0X11
#define K 			10         //数据位数


/*串口通讯数据转换共用体*/
union Swap   //发送数据转换使用
{
    short s;
    unsigned char swap[4];
};

/*壶口坐标数据*/
typedef struct POT_POSTION
{
	int left_x_position ;
	int left_y_position ;
	int middle_x_position ;
	int middle_y_position ;
	int right_x_position  ;
	int right_y_position  ;     
	unsigned char testRece4;
}POT_POSTION;


extern POT_POSTION pot_data;

// 数据发送
void SendData(POT_POSTION *pot_data);
void creatSendData(float *data , unsigned char *sendData , int length);

// 数据接收
short Parse_Data(unsigned char *receiveData ,int index);
unsigned char getCrc8(unsigned char *ptr, unsigned short len);
int usartReceiveOneData(int *left_x_pos,int *left_y_pos,int *middle_x_pos,int *middle_y_pos,int *right_x_pos,int *right_y_pos,unsigned char *p_crtlFlag);

#endif
