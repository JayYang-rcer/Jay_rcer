#include "vesc_can.h"
#include "main.h"
#include "can.h"

CAN_TxHeaderTypeDef TxMessage;

int16_t buffer_get_int16(uint8_t *buf, int32_t *index)
{
    int16_t result = ((uint16_t)buf[*index]) << 8 | ((uint16_t)buf[*index + 1]);
    (*index) += 2;
    return result;
}


uint16_t buffer_get_uint16(uint8_t *buf, int32_t *index)
{
    uint16_t result = ((uint16_t)buf[*index]) <<8 | ((uint16_t)buf[*index+1]);
	(*index)+=2;	
	return result;
}


int32_t buffer_get_int32(uint8_t *buf, int32_t *index)
{
    int32_t result = ((uint32_t)buf[*index]) << 24 | 
                     ((uint32_t)buf[*index+1]) << 16 |
                     ((uint32_t)buf[*index+2]) << 8 |
                     ((uint32_t)buf[*index+3]);
    (*index) += 4;
    return result; 
}


float buffer_get_float16(uint8_t *buf, int32_t *index)
{
    return (float)buffer_get_int16(buf,index);
}


float buffer_get_float32(uint8_t* buf,int32_t *index)
{
	return (float)buffer_get_int32(buf,index);
}


/**************************************
 * 限幅函数
 **************************************/
void assert_param_duty(float *duty)
{
	if( fabsf(*duty) > wtrcfg_VESC_COMMAND_DUTY_MAX )
		*duty = *duty > 0 ? wtrcfg_VESC_COMMAND_DUTY_MAX : - wtrcfg_VESC_COMMAND_DUTY_MAX;
}
void assert_param_current(float *current)
{
	if( fabsf(*current) > wtrcfg_VESC_COMMAND_CURRENT_MAX )
		*current = *current > 0 ? wtrcfg_VESC_COMMAND_CURRENT_MAX : - wtrcfg_VESC_COMMAND_CURRENT_MAX;
}
void assert_param_rpm(float *rpm)
{
	if( fabsf(*rpm) > wtrcfg_VESC_COMMAND_ERPM_MAX )
		*rpm = *rpm > 0 ? wtrcfg_VESC_COMMAND_ERPM_MAX : - wtrcfg_VESC_COMMAND_ERPM_MAX;
}
void assert_param_pos(float *pos)
{
	if( fabsf(*pos) > wtrcfg_VESC_COMMAND_POS_MAX )
		*pos = *pos > 0 ? wtrcfg_VESC_COMMAND_POS_MAX : - wtrcfg_VESC_COMMAND_POS_MAX;
}


/******************************************************************************************************
 *	can报文格式如下
 * 解释：| ID格式 | 帧格式 | 数据长度 |           扩展ID 29位            | 数据0byte  |...| 数据3byte  |
 * 位置：|   IDE  |   RTR  |    DLC   |              ExtId               | value高8位 |...| value低8位 |
 * 值  ：|   EXT  |  DATA  |    8     | 低八位为vesc_id,第八位以上为指令 | data >> 24 |...|    data    |
 ******************************************************************************************************/
static uint32_t txmailbox;
HAL_StatusTypeDef can_send_command(VESC_t *hvesc, const CAN_PACKET_ID id, float value,int all)
{
	uint8_t txbuf[8];
	int32_t data;
		
	TxMessage.DLC = (uint8_t)8 ; /* --- can set to 4 --- */
	TxMessage.RTR = CAN_RTR_DATA ;
	TxMessage.IDE = CAN_ID_EXT ;
	if( all )
		TxMessage.ExtId = id | ((uint32_t)CAN_PACKET_SET_DUTY << 8);
	else 
		TxMessage.ExtId = ( (uint32_t) ( (id) << 8) | 0XFF );
	
	switch( id )
	{
		case CAN_PACKET_SET_DUTY:
		{
		  /* --- duty: 1e6f ~ 1.0 ~ 100% --- */
		  /* --- value: [-100,100] 1(%) --- */
			assert_param_duty(&value);
			data = (int32_t)(value * 1e5f );
			txbuf[0] = data >> 24;
			txbuf[1] = data >> 16;
			txbuf[2] = data >> 8;
			txbuf[3] = data;
			break;
		}
		case CAN_PACKET_SET_CURRENT:
		{
			/* --- current: 1e3f ~ 1 A --- */
			/* --- value: [-current_max,-current_max] 1A --- */
			assert_param_current(&value);
			data = (int32_t)(value * 1e3f );
			txbuf[0] = data >> 24;
			txbuf[1] = data >> 16;
			txbuf[2] = data >> 8;
			txbuf[3] = data;
			break;
		}
		case CAN_PACKET_SET_RPM:
		{
			/* --- erpm: 1 ~ 1 rpm --- */
			/* --- value: [-erpm_max,erpm_max] 1rpm --- */
			assert_param_rpm(&value);
			data = (int32_t)(value);
			txbuf[0] = data >> 24;
			txbuf[1] = data >> 16;
			txbuf[2] = data >> 8;
			txbuf[3] = data;
			break;
		}
		case CAN_PACKET_SET_POS:
		{
			/* --- pos: 1e6f ~ 1 degree --- */
			/* --- value: [-inf,+inf] 1degree --- */
			assert_param_pos(&value);
			data = (int32_t)(value * 1e6f);
			txbuf[0] = data >> 24;
			txbuf[1] = data >> 16;
			txbuf[2] = data >> 8;
			txbuf[3] = data;
			break;
		}
		default: break;
	}
	if(HAL_CAN_AddTxMessage(hvesc->hcann,&TxMessage,txbuf,&txmailbox)!=HAL_OK){
		Error_Handler();
	}	
	return HAL_OK;
}


// void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
//    buffer[(*index)++] = number >> 24;
//    buffer[(*index)++] = number >> 16;
//    buffer[(*index)++] = number >> 8;
//    buffer[(*index)++] = number;
// }
// void vesc_can_set_duty(uint8_t controller_id, float duty)
// {
//     TxMessage.DLC = (uint8_t)8 ; /* --- can set to 4 --- */
// 	TxMessage.RTR = CAN_RTR_DATA ;
// 	TxMessage.IDE = CAN_ID_EXT ;
// 	TxMessage.ExtId = controller_id | ((uint32_t)CAN_PACKET_SET_RPM << 8);

//     int32_t send_index = 0;
//     uint8_t buffer[4];
//     buffer_append_int32(buffer, (int32_t)(duty * 100000), &send_index);
//     HAL_CAN_AddTxMessage(&hcan1, &TxMessage, buffer, &txmailbox);
// }
// void comm_can_set_rpm(uint8_t controller_id, float rpm) 
// {
//    int32_t send_index = 0;
//    uint8_t buffer[4];
//    buffer_append_int32(buffer, (int32_t)rpm, &send_index);
//    HAL_CAN_AddTxMessage(&hcan1, &TxMessage,  buffer, &txmailbox);
// }

