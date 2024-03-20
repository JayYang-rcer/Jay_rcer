#include "includes.h"


// 利用串口debug
void Debug_by_UART(void *pdata)
{
	while(1)
	{
	// debug定位功能
	#if LOCATION_DEBUG
	printf("[LOCALTION]: X: %fmm Y:%fmm YAW: %f° \r\n",
	ROBOT_REAL_POS_DATA.X_POS,
	ROBOT_REAL_POS_DATA.Y_POS,
	ROBOT_REAL_POS_DATA.YAW_POS);
	#endif
	
	// debug速度
	#if VELOCITY_DEBUG
	printf("[VELOCITY]: Vx_RPM: %d Vy_RPM:%d W_RPM: %d \r\n",
	ROBOT_TARGET_VELOCITY_DATA.Vx_RPM,
	ROBOT_TARGET_VELOCITY_DATA.Vy_RPM,
	ROBOT_TARGET_VELOCITY_DATA.W_RPM);
	printf("--------------------------------------------------------------------- \r\n");
	#endif
	
	// debug单个m3508电机（可修改不同的序号）
	#if M3508_DEBUG
	printf("[M3508]: ANGLE: %d RPM:%d CURRENT: %d \r\n",
	M3508_MOTOR_REAL_INFO.ANGLE,
	M3508_MOTOR_REAL_INFO.RPM,
	M3508_MOTOR_REAL_INFO.CURRENT);
	printf("--------------------------------------------------------------------- \r\n");
	#endif
	
	// debug路径跟踪
	#if DEBUG_PATH_TRACKING

	#endif
	
	// debug色标传感器
	#if DEBUG_COLOR_SENSOR
	printf("VERTICAL_LEFT_COLOR_SENSOR  : %d \r\n", Read_color(VERTICAL_LEFT_COLOR_SENSOR)) ;
	printf("ACLINIC_LEFT_COLOR_SENSOR   : %d \r\n", Read_color(ACLINIC_LEFT_COLOR_SENSOR))  ;
	printf("ACLINIC_RIGHT_COLOR_SENSOR  : %d \r\n", Read_color(ACLINIC_RIGHT_COLOR_SENSOR)) ;
	printf("VERTICAL_RIGHT_COLOR_SENSOR : %d \r\n", Read_color(VERTICAL_RIGHT_COLOR_SENSOR));
	printf("--------------------------------------------------------------------- \r\n");
	#endif
	
	// debug激光测距传感器
	#if DEBUG_LASER_RANGING
	printf("RANGING_DISTANCE : %f mm\r\n", RANGING_DISTANCE);
	printf("--------------------------------------------------------------------- \r\n");
	#endif

	// debug限位开关
	#if DEBUG_LIMIT_SWITCH
	printf("UNDER_ARM_RESET_LIMIT_SWITCH     : %d \r\n", Read_limit_switch(UNDER_ARM_RESET_LIMIT_SWITCH));
	printf("TURNPLATE_RESET_LIMIT_SWITCH     : %d \r\n", Read_limit_switch(TURNPLATE_RESET_LIMIT_SWITCH));
	printf("PULL_RESET_LIMIT_SWITCH          : %d \r\n", Read_limit_switch(PULL_RESET_LIMIT_SWITCH));
	printf("ARM_RESET_LIMIT_SWITCH           : %d \r\n", Read_limit_switch(ARM_RESET_LIMIT_SWITCH));
	printf("MOVE_ARM_RESET_LIMIT_SWITCH      : %d \r\n", Read_limit_switch(MOVE_ARM_RESET_LIMIT_SWITCH));
	printf("MOVE_ARM_MIDDLE_LIMIT_SWITCH     : %d \r\n", Read_limit_switch(MOVE_ARM_MIDDLE_LIMIT_SWITCH));
	printf("ARM_ARRIVE_LIMIT_SWITCH          : %d \r\n", Read_limit_switch(ARM_ARRIVE_LIMIT_SWITCH));
	printf("PULL_ARRIVE_LIMIT_SWITCH         : %d \r\n", Read_limit_switch(PULL_ARRIVE_LIMIT_SWITCH));

	printf("--------------------------------------------------------------------- \r\n");
	#endif
	
	#if DEBUG_PS2
	printf("PS2_KEY: %-5d", PS2_KEY);
	printf("PS2_LX : %-5d", PS2_LX);		
	printf("PS2_LY : %-5d", PS2_LY);
	printf("PS2_RX : %-5d", PS2_RX);	
	printf("PS2_RY : %-5d\r\n", PS2_RY);	
	printf("--------------------------------------------------------------------- \r\n");	
	#endif
	OSTimeDly(5);
  };
}


