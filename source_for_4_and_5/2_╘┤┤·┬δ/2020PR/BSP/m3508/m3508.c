#include "includes.h"

/*********************
 velocity 机器人的速度 
 speed    电机轴rpm 
 RPM      电机转子rpm
 *********************/

M3508_REAL_INFO M3508_CHASSIS_MOTOR_REAL_INFO[4];   // 1-4分别对应顺时针方向的底盘电机
M3508_REAL_INFO M3508_ARM_MOTOR_REAL_INFO;          // 夹球电机的参数结构体
M3508_REAL_INFO GM6020_TURN_MOTOR_REAL_INFO;         // 转盘电机的参数结构体
M3508_REAL_INFO M2006_TURN_MOTOR_REAL_INFO;

PID M3508_ARM_MOTOR_PID_RPM;            // 夹球电机速度环PID
PID GM6020_TURN_MOTOR_PID_RPM;           // GM6020电机速度环PID
PID M2006_TURN_MOTOR_PID_POS;
PID M3508_CHASSIS_MOTOR_PID_RPM[4];
PID M2006_TURN_MOTOR_PID_RPM;           // 转盘电机速度环PID

int16_t ARM_TURN_MOTOR_TARGET_RPM = 0;    // 夹球电机目标速度
int16_t TURN_MOTOR_TARGET_RPM = 0;    // 云台电机目标速度

// M3508初始化
void m3508_motor_init(void)
{
	// PID初始化
	
	PID_parameter_init(&M3508_ARM_MOTOR_PID_RPM       , 10.0, 1.0, 0.0, 16384, 16384, -1);  // 夹球电机速度环PID初始化
	PID_parameter_init(&GM6020_TURN_MOTOR_PID_RPM     , 20.0,50.0, 30.0, 30000, 30000, -1);
//	PID_parameter_init(&GM6020_TURN_MOTOR_PID_RPM     , 100.0, 0.0,0.0, 30000, 30000, -1);
	PID_parameter_init(&M2006_TURN_MOTOR_PID_POS      ,   90, 0.0,  40, 10000,   500, -1);
	PID_parameter_init(&M2006_TURN_MOTOR_PID_RPM      , 10.0, 1.0, 0.0, 10000, 10000, -1);  // 转盘电机速度环PID初始化
	PID_parameter_init(&M3508_CHASSIS_MOTOR_PID_RPM[0], 10.0, 1.0, 0.0, 16384, 16384, -1);
	PID_parameter_init(&M3508_CHASSIS_MOTOR_PID_RPM[1], 10.0, 1.0, 0.0, 16384, 16384, -1);
	PID_parameter_init(&M3508_CHASSIS_MOTOR_PID_RPM[2], 10.0, 1.0, 0.0, 16384, 16384, -1);
	PID_parameter_init(&M3508_CHASSIS_MOTOR_PID_RPM[3], 10.0, 1.0, 0.0, 16384, 16384, -1);
	
}


// 利用电机通过CAN反馈的数据更新m3508的状态信息
// 接受频率：1kHz
void m3508_update_m3508_info(CanRxMsg *msg)
{
	switch(msg -> StdId)  // 检测标准ID
	{
			
		case M3508_ARM_MOTOR_ID:
		{ 
			M3508_ARM_MOTOR_REAL_INFO.ANGLE   = (msg -> Data[0] << 8) | msg -> Data[1];  // 转子机械角度
			M3508_ARM_MOTOR_REAL_INFO.RPM     = (msg -> Data[2] << 8) | msg -> Data[3];  // 实际转子转速
			M3508_ARM_MOTOR_REAL_INFO.CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
			// 进行角度积分
			m3508_ARM_angle_integral(&M3508_ARM_MOTOR_REAL_INFO);
		}; break;
			
    case M3508_CHASSIS_MOTOR_ID_1:
		{ 
			M3508_CHASSIS_MOTOR_REAL_INFO[0].ANGLE   = (msg -> Data[0] << 8) | msg -> Data[1];  // 转子机械角度
			M3508_CHASSIS_MOTOR_REAL_INFO[0].RPM     = (msg -> Data[2] << 8) | msg -> Data[3];  // 实际转子转速
			M3508_CHASSIS_MOTOR_REAL_INFO[0].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
		}; break;
		
		case M3508_CHASSIS_MOTOR_ID_2:
		{ 
			M3508_CHASSIS_MOTOR_REAL_INFO[1].ANGLE   = (msg -> Data[0] << 8) | msg -> Data[1];  // 转子机械角度
			M3508_CHASSIS_MOTOR_REAL_INFO[1].RPM     = (msg -> Data[2] << 8) | msg -> Data[3];  // 实际转子转速
			M3508_CHASSIS_MOTOR_REAL_INFO[1].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
		}; break;
		
		case M3508_CHASSIS_MOTOR_ID_3:
		{ 
			M3508_CHASSIS_MOTOR_REAL_INFO[2].ANGLE   = (msg -> Data[0] << 8) | msg -> Data[1];  // 转子机械角度
			M3508_CHASSIS_MOTOR_REAL_INFO[2].RPM     = (msg -> Data[2] << 8) | msg -> Data[3];  // 实际转子转速
			M3508_CHASSIS_MOTOR_REAL_INFO[2].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
		}; break;	
		
		case M3508_CHASSIS_MOTOR_ID_4:
		{ 
			M3508_CHASSIS_MOTOR_REAL_INFO[3].ANGLE   = (msg -> Data[0] << 8) | msg -> Data[1];  // 转子机械角度
			M3508_CHASSIS_MOTOR_REAL_INFO[3].RPM     = (msg -> Data[2] << 8) | msg -> Data[3];  // 实际转子转速
			M3508_CHASSIS_MOTOR_REAL_INFO[3].CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
			
		}; break;
		
		case M2006_TURN_MOTOR_ID:
		{ 
			M2006_TURN_MOTOR_REAL_INFO.ANGLE   = (msg -> Data[0] << 8) | msg -> Data[1];  // 转子机械角度
			M2006_TURN_MOTOR_REAL_INFO.RPM     = (msg -> Data[2] << 8) | msg -> Data[3];  // 实际转子转速
			M2006_TURN_MOTOR_REAL_INFO.CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
			// 进行角度积分
			m2006_TURN_MOTOR_angle_integral(&M2006_TURN_MOTOR_REAL_INFO);	
		}; break;
		
		case GM6020_MOTOR_ID:
		{ 
			GM6020_TURN_MOTOR_REAL_INFO.ANGLE   = (msg -> Data[0] << 8) | msg -> Data[1];  // 转子机械角度
			GM6020_TURN_MOTOR_REAL_INFO.RPM     = (msg -> Data[2] << 8) | msg -> Data[3];  // 实际转子转速
			GM6020_TURN_MOTOR_REAL_INFO.CURRENT = (msg -> Data[4] << 8) | msg -> Data[5];  // 实际转矩电流
			// 进行角度积分
			
		}; break;

		default: break;
	}
}


// 通过CAN1发送底盘m3508的电流
// 接受频率：1kHz
// 转子角度范围值：0-8191（对应0到360度）
// 转子转速单位为RPM
// 电机温度单位为摄氏度
void m2006_send_motor_currents(void)
{
	CanTxMsg tx_message;

	// 配置控制段
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
	
	// 配置仲裁段和数据段
	
	tx_message.StdId = 0x200;  // 用于ID为 1 2 3 4 的电机
	tx_message.Data[0] = (uint8_t)(M3508_CHASSIS_MOTOR_REAL_INFO[0].TARGET_CURRENT >> 8);
	tx_message.Data[1] = (uint8_t) M3508_CHASSIS_MOTOR_REAL_INFO[0].TARGET_CURRENT;
	tx_message.Data[2] = (uint8_t)(M3508_CHASSIS_MOTOR_REAL_INFO[1].TARGET_CURRENT >> 8);
	tx_message.Data[3] = (uint8_t) M3508_CHASSIS_MOTOR_REAL_INFO[1].TARGET_CURRENT;
	tx_message.Data[4] = (uint8_t)(M3508_CHASSIS_MOTOR_REAL_INFO[2].TARGET_CURRENT >> 8);
	tx_message.Data[5] = (uint8_t) M3508_CHASSIS_MOTOR_REAL_INFO[2].TARGET_CURRENT;
	tx_message.Data[6] = (uint8_t)(M3508_CHASSIS_MOTOR_REAL_INFO[3].TARGET_CURRENT >> 8);
	tx_message.Data[7] = (uint8_t) M3508_CHASSIS_MOTOR_REAL_INFO[3].TARGET_CURRENT;
	
	CAN_Transmit(CAN1, &tx_message);
}

void m3508_send_motor_currents(void)
{
	CanTxMsg tx_message;

	// 配置控制段
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
	
	// 配置仲裁段和数据段	
	tx_message.StdId = 0x1ff;  // 用于ID为 5 6 7 8 的电机
	tx_message.Data[0] = (uint8_t)(M3508_ARM_MOTOR_REAL_INFO.TARGET_CURRENT >> 8);
	tx_message.Data[1] = (uint8_t) M3508_ARM_MOTOR_REAL_INFO.TARGET_CURRENT;
	tx_message.Data[2] = (uint8_t)(M2006_TURN_MOTOR_REAL_INFO.TARGET_CURRENT >> 8);
	tx_message.Data[3] = (uint8_t) M2006_TURN_MOTOR_REAL_INFO.TARGET_CURRENT;


	CAN_Transmit(CAN1, &tx_message);
}

void GM6020_send_motor_currents(void)
{
	CanTxMsg tx_message;

	// 配置控制段
	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 0x08;
	
	// 配置仲裁段和数据段	
	tx_message.StdId = 0x2ff;  // 用于ID为 5 6 7 8 的电机

	tx_message.Data[2] = (uint8_t)(GM6020_TURN_MOTOR_REAL_INFO.TARGET_CURRENT >> 8)&0xff;
	tx_message.Data[3] = (uint8_t)(GM6020_TURN_MOTOR_REAL_INFO.TARGET_CURRENT)&0xff;
	tx_message.Data[4] = (uint8_t)(GM6020_TURN_MOTOR_REAL_INFO.TARGET_CURRENT >> 8)&0xff;
	tx_message.Data[5] = (uint8_t)(GM6020_TURN_MOTOR_REAL_INFO.TARGET_CURRENT)&0xff;

	CAN_Transmit(CAN1, &tx_message);
}



// 机械臂电机角度积分
void m3508_ARM_angle_integral(M3508_REAL_INFO *M3508_MOTOR)
{
	static uint16_t last_angle = 0;
	static u8 first_enter_flag = 1;
	float delta_pos = 0;
	
	// 记录第一次进入时的数据
	if(first_enter_flag)
	{
		last_angle = M3508_MOTOR->ANGLE;
		first_enter_flag = 0;
		return;
	}
	
	// 计算变化的角度
	if(M3508_MOTOR->RPM >= 0)
	{
		if(M3508_MOTOR->ANGLE < last_angle)
		{
			if(abs(8191 + M3508_MOTOR->ANGLE - last_angle) < 1250)  // 利用两次CAN接收时间电机最大转动角度进行滤波
				delta_pos = ((float)(8191 + M3508_MOTOR->ANGLE - last_angle) / 8191.0f) * (360.0f/19.0f);
		}
		else
		{
			delta_pos = ((float)(M3508_MOTOR->ANGLE - last_angle) / 8191.0f) * (360.0f/19.0f);
		}
		
		// 滤波
		if(delta_pos > 0) 
			M3508_MOTOR->REAL_ANGLE += delta_pos;  // 积分
	}
	else if(M3508_MOTOR->RPM < 0)
	{
		if(M3508_MOTOR->ANGLE > last_angle)
		{
			if(abs(8191 - M3508_MOTOR->ANGLE + last_angle) < 1250)  // 利用两次CAN接收时间电机最大转动角度进行滤波			
				delta_pos = ((float)(8191 - M3508_MOTOR->ANGLE + last_angle) / 8191.0f) * (360.0f/19.0f);
		}	
		else
		{
			delta_pos = ((float)(M3508_MOTOR->ANGLE - last_angle) / 8191.0f) * (360.0f/19.0f);
		}
		
		// 滤波
		if(delta_pos < 0) 
			M3508_MOTOR->REAL_ANGLE += delta_pos;  // 积分
	}

	// 存储角度值
	last_angle = M3508_MOTOR->ANGLE;

}



// 转盘电机角度积分
void m2006_TURN_MOTOR_angle_integral(M3508_REAL_INFO *M3508_MOTOR)
{
	static uint16_t last_angle = 0;
	static u8 first_enter_flag = 1;
	float delta_pos = 0;
	
	// 记录第一次进入时的数据
	if(first_enter_flag)
	{
		last_angle = M3508_MOTOR->ANGLE;
		first_enter_flag = 0;
		return;
	}
	
	// 计算变化的角度
	if(M3508_MOTOR->RPM >= 0)
	{
		if(M3508_MOTOR->ANGLE < last_angle)
		{
			if(abs(8191 + M3508_MOTOR->ANGLE - last_angle) < 1250)  // 利用两次CAN接收时间电机最大转动角度进行滤波
				delta_pos = ((float)(8191 + M3508_MOTOR->ANGLE - last_angle) / 8191.0f) * ((360.0f*15)/(36.0f*66));
		}
		else
		{
			delta_pos = ((float)(M3508_MOTOR->ANGLE - last_angle) / 8191.0f) * ((360.0f*15)/(36.0f*66));
		}
		
		// 滤波
		if(delta_pos > 0) 
			M3508_MOTOR->REAL_ANGLE += delta_pos;  // 积分
	}
	else if(M3508_MOTOR->RPM < 0)
	{
		if(M3508_MOTOR->ANGLE > last_angle)
		{
			if(abs(8191 - M3508_MOTOR->ANGLE + last_angle) < 1250)  // 利用两次CAN接收时间电机最大转动角度进行滤波			
				delta_pos = ((float)(8191 - M3508_MOTOR->ANGLE + last_angle) / 8191.0f) * ((360.0f*15)/(36.0f*66));
		}	
		else
		{
			  delta_pos = ((float)(M3508_MOTOR->ANGLE - last_angle) / 8191.0f) * ((360.0f*15)/(36.0f*66));
		}
		// 滤波
		if(delta_pos < 0) 
			M3508_MOTOR->REAL_ANGLE += delta_pos;  // 积分
	}

	// 存储角度值
	last_angle = M3508_MOTOR->ANGLE;

}
