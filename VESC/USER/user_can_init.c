#include "user_can_init.h"
#include "can.h"
#include "stm32f4xx.h"
#include "gpio.h"
#include "stm32f4xx_hal_can.h"


uint8_t   RxData[8];
CAN_TxHeaderTypeDef			TxHeader;
CAN_RxHeaderTypeDef 		RxHeader;


static motor_measure_t      motor_chassis[7];


void User_Open_Can(void)
{
	CAN_FilterTypeDef Filter;
	
	Filter.FilterActivation = ENABLE;
    Filter.FilterMode = CAN_FILTERMODE_IDMASK;
    Filter.FilterScale = CAN_FILTERSCALE_32BIT;
    Filter.FilterIdHigh = 0x0000;
    Filter.FilterIdLow = 0x0000;
    Filter.FilterMaskIdHigh = 0x0000;
    Filter.FilterMaskIdLow = 0x0000;
    Filter.FilterBank = 0;
    Filter.FilterFIFOAssignment = CAN_RX_FIFO0;
	Filter.SlaveStartFilterBank = 14;
	
	if (HAL_CAN_ConfigFilter(&hcan1, &Filter) != HAL_OK)
	{
			/* Filter configuration Error */
			Error_Handler();
	}
		
	if (HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}
		
		/*##-4- Activate CAN RX notification #######################################*/
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}
	
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}
	
	TxHeader.IDE=CAN_ID_EXT;    //使用标准帧
	TxHeader.RTR=CAN_RTR_DATA;  //数据帧
	TxHeader.DLC=8; 
}



//hal库CAN回调函数,接收电机数据
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxHeader, RxData);
	
	switch(RxHeader.StdId)
	{
		case CAN_M3508_M1_ID:
		case CAN_M3508_M2_ID:
		case CAN_M3508_M3_ID:
		case CAN_M3508_M4_ID:
		{
			static uint8_t i=0;
			i = RxHeader.StdId - CAN_M3508_M1_ID;
			get_motor_measure(&motor_chassis[i], RxData);
			
		}
	}
}


//电机数据读取
void get_motor_measure(motor_measure_t *ptr, uint8_t data[8])                                    
{                                                                   
    ptr->last_ecd = ptr->ecd;                                   
    ptr->ecd = (uint16_t)(data[0] << 8 | data[1]);            
    ptr->speed_rpm = (uint16_t)(data[2] << 8 | data[3]);      
    ptr->given_current = (uint16_t)(data[4] << 8 | data[5]);  
    ptr->temperate = data[6];                                   
}


void M3508_Send_Currents(int16_t motor1,int16_t motor2, int16_t motor3,int16_t motor4)
{
	uint8_t TxData[8];
	uint32_t Send_Mail_Box;
	TxHeader.StdId = CAN_CHASSIS_ALL_ID;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 0x08;
	
	TxData[0] = motor1 >> 8;
	TxData[1] = motor1;
	TxData[2] = motor2 >> 8;
	TxData[3] = motor2;
	TxData[4] = motor3 >> 8;
	TxData[5] = motor3;
	TxData[6] = motor4 >> 8;
	TxData[7] = motor4;
	
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &Send_Mail_Box);
}


void M3508_Send_Currents_Reset(void)
{
	uint8_t TxData[8];
	uint32_t Send_Mail_Box;
	TxHeader.StdId = 0x700;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = 0x08;
	
	TxData[0] = 0 >> 8;
	TxData[1] = 0;
	TxData[2] = 0 >> 8;
	TxData[3] = 0;
	TxData[4] = 0 >> 8;
	TxData[5] = 0;
	TxData[6] = 0 >> 8;
	TxData[7] = 0;
	
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &Send_Mail_Box);
}

//返回 3508电机数据指针
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i)
{
    return &motor_chassis[(i & 0x03)];
}


