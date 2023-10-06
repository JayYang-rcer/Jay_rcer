#ifndef __VESC_CAN_H
#define __VESC_CAN_H
#include "main.h"

#ifndef wtrcfg_VESC_COMMAND_DUTY_MAX
#define wtrcfg_VESC_COMMAND_DUTY_MAX 100
#endif 
#ifndef wtrcfg_VESC_COMMAND_CURRENT_MAX
#define wtrcfg_VESC_COMMAND_CURRENT_MAX 10
#endif 
#ifndef wtrcfg_VESC_COMMAND_POS_MAX
#define wtrcfg_VESC_COMMAND_POS_MAX 360
#endif 
#ifndef wtrcfg_VESC_COMMAND_ERPM_MAX
#define wtrcfg_VESC_COMMAND_ERPM_MAX 20000
#endif 
#ifndef wtrcfg_VESC_UART_TIMEOUT 
#define wtrcfg_VESC_UART_TIMEOUT 0xff
#endif



typedef enum {
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL
}
CAN_PACKET_ID;


typedef struct{
	CAN_HandleTypeDef * hcann;
	uint8_t controller_id;
}VESC_t;

void assert_param_pos(float *pos);
void assert_param_rpm(float *rpm);
void assert_param_current(float *current);
void assert_param_duty(float *duty);

HAL_StatusTypeDef can_send_command(VESC_t *hvesc, const CAN_PACKET_ID id, float value,int all);
float buffer_get_float32(uint8_t* buf,int32_t *index);
float buffer_get_float16(uint8_t *buf, int32_t *index);
int32_t buffer_get_int32(uint8_t *buf, int32_t *index);
uint16_t buffer_get_uint16(uint8_t *buf, int32_t *index);
int16_t buffer_get_int16(uint8_t *buf, int32_t *index);


void comm_can_set_rpm(uint8_t controller_id, float rpm);
void vesc_can_set_duty(uint8_t controller_id, float duty);
void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index);

#endif


