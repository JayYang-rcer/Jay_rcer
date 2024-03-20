#include "headfiles.h"

/*********************
 postion  电机位置
 speed    电机速度
 torque   电机力矩
 *********************/
AK80_REAL_INFO MIT_DRIVER_REAL_INFO[2];
PID MIT_DRIVER_PID_INFO[2];


void Mit_Driver_Init(void)
{
	PID_parameter_init(&MIT_DRIVER_PID_INFO[0], 1.0f, 0.1f, 0, I_MAX, I_MAX, -1);
	PID_parameter_init(&MIT_DRIVER_PID_INFO[1], 0.8f, 0.05f, 0, I_MAX, I_MAX, -1);
	
	ak80_control_cmd(MIT_DRIVER_ID_1, 0x01);
	ak80_control_cmd(MIT_DRIVER_ID_2, 0x01);
}

//取最大
float fmaxf(float a, float b)
{
		return a>=b?a:b;
}
//取最小
float fminf(float a, float b)
{
		return a<=b?a:b;
}
	
//浮点型转整形
int float_to_uint(float x, float x_min, float x_max, int bits) /// Converts a float to an unsigned int, given range and number of bits /// 
{
		float span = x_max - x_min; 
		float offset = x_min; 
		return (int) ((x-offset)*((float)((1<<bits)-1))/span); 
}

/*
p_des 目标位置
v_des 目标速度
Kp    位置环参数
kd    速度环参数
t_ff  目标扭矩
*/
 
//控制电机
void AK80_motion_control(u8 ID,float p_des, float v_des, float kp, float kd, float t_ff)/// limit data to be within bounds ///
{
		// 配置控制段
		CanTxMsg tx_message;
		// 配置控制段
		tx_message.IDE = CAN_Id_Standard;
		tx_message.RTR = CAN_RTR_Data;
		tx_message.DLC = 0x08;
	
		// 配置仲裁段和数据段	
		tx_message.StdId = ID;
		p_des = fminf(fmaxf(P_MIN, p_des), P_MAX);
		v_des = fminf(fmaxf(V_MIN, v_des), V_MAX);
		kp = fminf(fmaxf(Kp_MIN, kp), Kp_MAX);
		kd = fminf(fmaxf(Kd_MIN, kd), Kd_MAX);
		t_ff = fminf(fmaxf(T_MIN, t_ff), T_MAX);
		/// convert floats to unsigned ints ///
		int p_int = float_to_uint(p_des, P_MIN, P_MAX, 16);
		int v_int = float_to_uint(v_des, V_MIN, V_MAX, 12);
		int kp_int = float_to_uint(kp, Kp_MIN, Kp_MAX, 12);
		int kd_int = float_to_uint(kd, Kd_MIN, Kd_MAX, 12);
		int t_int = float_to_uint(t_ff, T_MIN, T_MAX, 12);
		/// pack ints into the can buffer ///
		tx_message.Data[0] = p_int>>8; //位置高 8
		tx_message.Data[1] = p_int&0xFF; //位置低 8
		tx_message.Data[2] = v_int>>4; //速度高 8 位
		tx_message.Data[3] = ((v_int&0xF)<<4)|(kp_int>>8); //速度低 4 位 KP 高 4 位
		tx_message.Data[4] = kp_int&0xFF; //KP 低 8 位
		tx_message.Data[5] = kd_int>>4; //Kd 高 8 位
		tx_message.Data[6] = ((kd_int&0xF)<<4)|(t_int>>8); //KP 低 4 位扭矩高 4 位
		tx_message.Data[7] = t_int&0xff; //扭矩低 8 位
		
		CAN_Transmit(CAN2, &tx_message);
}
	
//进入电机控制模式，必须调用后才可以用AK80_motion_control（）控制电机
void ak80_control_cmd(uint8_t ID,uint8_t cmd)/// limit data to be within bounds ///
{
		CanTxMsg tx_message;
		// 配置控制段
		tx_message.IDE = CAN_Id_Standard;
		tx_message.RTR = CAN_RTR_Data;
		tx_message.DLC = 0x08;
	
		// 配置仲裁段和数据段	
		tx_message.StdId = ID;  // 用于ID为 1 的电机
		/// pack ints into the can buffer ///
		tx_message.Data[0] = (uint8_t)(0xFF);
		tx_message.Data[1] = (uint8_t)(0xFF);
		tx_message.Data[2] = (uint8_t)(0xFF);
		tx_message.Data[3] = (uint8_t)(0xFF);
		tx_message.Data[4] = (uint8_t)(0xFF);
		tx_message.Data[5] = (uint8_t)(0xFF);
		tx_message.Data[6] = (uint8_t)(0xFF);
	
	  switch(cmd)
    {
        case CMD_MOTOR_MODE:
            tx_message.Data[7] = (uint8_t)(0xFC);
            break;
        
        case CMD_RESET_MODE:
            tx_message.Data[7] = (uint8_t)(0xFD);
        break;
        
        case CMD_ZERO_POSITION:
            tx_message.Data[7] = (uint8_t)(0xFE);
        break;
        
        default:
        return; /* 直接退出函数 */
    }
		CAN_Transmit(CAN2, &tx_message);
}



// mit电流发送
void MIT_Driver_Current_Send(u8 ID, float t_ff)
{
	int t_int;
	CanTxMsg tx_message;
	
	// 配置控制段
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
	tx_message.StdId = ID;
	t_int = float_to_uint(t_ff, T_MIN, T_MAX, 12);
	tx_message.Data[0] = 0x7F;
	tx_message.Data[1] = 0xFF;
	tx_message.Data[2] = 0x7F;
	tx_message.Data[3] = 0xF0;
	tx_message.Data[4] = 0x00;
	tx_message.Data[5] = 0x00;
	tx_message.Data[6] = t_int>>8;
	tx_message.Data[7] = t_int&0xFF; //扭矩低 8 位
	
	CAN_Transmit(CAN2, &tx_message);
}


	
//整形转浮点型
float uint_to_float(int x_int, float x_min, float x_max, int bits)
{ 	
	  /// converts unsigned int to float, given range and number of bits ///
		float span = x_max - x_min;
		float offset = x_min;
		return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}
	
//反馈电机信息	
void AK80_update_info(CanRxMsg *msg)
{ 	
	  int p_int;
	  int v_int;
	  int i_int;
		switch(msg->Data[0])  // 检测标准ID
		{
			case MIT_DRIVER_ID_1:
			{ 
				p_int = (msg->Data[1]<<8)|msg->Data[2]; 			//电机位置数据
				v_int = (msg->Data[3]<<4)|(msg->Data[4]>>4); 	//电机速度数据
				i_int = ((msg->Data[4]&0xF)<<8)|msg->Data[5]; //电机扭矩数据
				
				MIT_DRIVER_REAL_INFO[0].ANGLE   = uint_to_float(p_int, MIT_P_MIN, MIT_P_MAX, 16);			// 转子机械角度
				MIT_DRIVER_REAL_INFO[0].V_angle     = uint_to_float(v_int, MIT_V_MIN, MIT_V_MAX, 12);	// 实际转子转速
				MIT_DRIVER_REAL_INFO[0].CURRENT = uint_to_float(i_int, -I_MAX, I_MAX, 12);						// 实际转矩电流

			}; break;
			
			case MIT_DRIVER_ID_2:
			{ 
				p_int = (msg->Data[1]<<8)|msg->Data[2]; 			//电机位置数据
				v_int = (msg->Data[3]<<4)|(msg->Data[4]>>4); 	//电机速度数据
				i_int = ((msg->Data[4]&0xF)<<8)|msg->Data[5]; //电机扭矩数据
				
				MIT_DRIVER_REAL_INFO[1].ANGLE   = uint_to_float(p_int, MIT_P_MIN, MIT_P_MAX, 16);			// 转子机械角度
				MIT_DRIVER_REAL_INFO[1].V_angle     = uint_to_float(v_int, MIT_V_MIN, MIT_V_MAX, 12);	// 实际转子转速
				MIT_DRIVER_REAL_INFO[1].CURRENT = uint_to_float(i_int, -I_MAX, I_MAX, 12);						// 实际转矩电流

			}; break;
			
			default:break;
		}

}




