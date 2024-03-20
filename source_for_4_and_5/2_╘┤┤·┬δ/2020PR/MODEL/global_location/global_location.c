#include "includes.h"


// 真实位置数据
ROBOT_REAL_POS ROBOT_REAL_POS_DATA = {0, 0, 0};

// 偏航角位置偏置量
float OFFSET_YAW = 0;
	
float SELF_GL_YAW_INIT_VALUE = 0;  // 陀螺仪初始值


// 全场定位初始化
void Location_init(void)
{ 
	#if USE_ACTION_GL 
	
	Action_Uart_init(115200);
	gyro_UART4_init(115200);
	SELF_GL_YAW_INIT_VALUE += ((float)GYRO_ANGLE.Angle[2] / 32768 * 180); 
	
	
	#endif
	
	#if USE_SELF_GL 
	
	// 初始化底层配置
	encoder_TIM4_init();
  encoder_TIM8_init();
	gyro_UART4_init(115200);
//		gyro_UART4_init(115200);
	// 等待陀螺仪启动与发送数据 读取陀螺仪Z轴初始角度
	// 在主循环开始前延时，不影响时序
	
	delay_ms(500);

	// 获取初始偏航角

	SELF_GL_YAW_INIT_VALUE += ((float)GYRO_ANGLE.Angle[2] / 32768 * 180); 		
	
	ROBOT_REAL_POS_DATA.X_POS += ROBOT_SZ_POS_OFFSET_X;
	ROBOT_REAL_POS_DATA.Y_POS += ROBOT_SZ_POS_OFFSET_Y;
	
	#endif
}

#if Action_Location	
// 在串口3中断中使用
void USART3_IRQHandler(void)
{
	static uint8_t ch;
	static union
	{
	 uint8_t data[24];
	 float ActVal[6];
	}posture;
	
	static uint8_t count = 0;
  static uint8_t i = 0;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)   
	{
		ch = USART_ReceiveData(USART3);

		switch(count)
		 {
			 case 0: 
			 {
				 if(ch == 0x0d) count ++;
				 else count = 0;
			 }
			 break;
				 
			 case 1:
			 {
				 if(ch == 0x0a)
				 {
					 i = 0;
					 count ++;
				 }
				 else if(ch == 0x0d);
				 else count = 0;
			 }
				 break;
				 
			 case 2:
			 {
				 posture.data[i] = ch;
			   i ++;
			   if(i >= 24)
				 {
					 i = 0;
					 count ++;
				 }
			 }
			 break;
				 
			 case 3:
			 {
				 if(ch == 0x0a) count++;
				 else
					 count = 0;
			 }
				 break;
				 
			 case 4:
			 {
				 if(ch == 0x0d)
				 {					 
          //更新传感器数据					 
					Update_Action_gl_position(posture.ActVal);
				 }
			   count = 0;
			 }
				 break;
			 
			 default:
			 {
				 count = 0;
			 }
			   break;		 
		 }
		 
		 USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
	}
}
// action全场定位模块定位数据
ACTION_GL_POS ACTION_GL_POS_DATA = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 更新action全场定位模块的值
void Update_Action_gl_position(float value[6])
{
	// 储存上一次的值
	ACTION_GL_POS_DATA.LAST_POS_X = ACTION_GL_POS_DATA.POS_X;
	ACTION_GL_POS_DATA.LAST_POS_Y = ACTION_GL_POS_DATA.POS_Y;	
	
	// 记录这次的值
	ACTION_GL_POS_DATA.ANGLE_Z = value[0];  // 有用
	ACTION_GL_POS_DATA.ANGLE_X = value[1];
	ACTION_GL_POS_DATA.ANGLE_Y = value[2];
	ACTION_GL_POS_DATA.POS_X   = value[3];  // 有用
	ACTION_GL_POS_DATA.POS_Y   = value[4];  // 有用
	ACTION_GL_POS_DATA.W_Z     = value[5];
	
	// 差分运算
	ACTION_GL_POS_DATA.DELTA_POS_X = ACTION_GL_POS_DATA.POS_X - ACTION_GL_POS_DATA.LAST_POS_X;
	ACTION_GL_POS_DATA.DELTA_POS_Y = ACTION_GL_POS_DATA.POS_Y - ACTION_GL_POS_DATA.LAST_POS_Y;	

	// 偏航角直接赋值（逆时针为正，顺时针为负）
  ROBOT_REAL_POS_DATA.YAW_POS = ACTION_GL_POS_DATA.ANGLE_Z - OFFSET_YAW;
	
	// 累加得出最终真实位置
	robot.x += ACTION_GL_POS_DATA.DELTA_POS_Y;
	ROBOT_REAL_POS_DATA.Y_POS += ACTION_GL_POS_DATA.DELTA_POS_X;
	ROBOT_REAL_POS_DATA.X_POS = -robot.x;
	
}
#endif

#if Self_Location
// 更新自制全场定位模块的值
// 在task中使用
void Update_self_gl_position(void)
{
	static float last_yaw = 0;  // 上一次的偏航角
	float delta_rad = 0;        // 偏航角弧度差
	float model_yaw = 0;        // 模块坐标系偏航角


	// 计算编码器行进的路程值
	Sensor.DELTA_DIS_X = Sensor.CNT_X * DISTANCE_PER_CNT;
	Sensor.DELTA_DIS_Y = Sensor.CNT_Y * DISTANCE_PER_CNT;
	
	Sensor.DIS_X += Sensor.CNT_X;
	Sensor.DIS_Y += Sensor.CNT_Y;
	
	// 计算模块坐标系偏航角
	model_yaw = ROBOT_REAL_POS_DATA.YAW_POS + YAW0;

	// 限制范围(-180， 180]	
	if(model_yaw > 180)        model_yaw = -180 + (model_yaw - 180);
	else if(model_yaw <= -180) model_yaw =  180 + (model_yaw + 180);
	
	
	// 计算偏航角弧度差并保存当前偏航角
	if(last_yaw*ROBOT_REAL_POS_DATA.YAW_POS >= 0)
	{
		delta_rad = (ROBOT_REAL_POS_DATA.YAW_POS - last_yaw) / 180.0f * PI;
	}
	else
	{
		if(ABS(last_yaw) + ABS(ROBOT_REAL_POS_DATA.YAW_POS) <= 180) delta_rad = (ROBOT_REAL_POS_DATA.YAW_POS - last_yaw) / 180.0f * PI;
		else
		{
			if((ROBOT_REAL_POS_DATA.YAW_POS - last_yaw) > 0)
			{
				delta_rad = (ROBOT_REAL_POS_DATA.YAW_POS - last_yaw - 360) / 180.0f * PI;
			}
			else if((ROBOT_REAL_POS_DATA.YAW_POS - last_yaw) < 0)
			{
				delta_rad = (ROBOT_REAL_POS_DATA.YAW_POS - last_yaw + 360) / 180.0f * PI;
			}
		}
	}
	last_yaw = ROBOT_REAL_POS_DATA.YAW_POS;
	
	// 运动学方程
	ROBOT_REAL_POS_DATA.Y_POS += (Sensor.DELTA_DIS_X - Lx * delta_rad) * cos(model_yaw / 180.0f * PI) - 
									             (Sensor.DELTA_DIS_Y + Ly * delta_rad) * sin(model_yaw / 180.0f * PI);
	
	ROBOT_REAL_POS_DATA.X_POS += (Sensor.DELTA_DIS_X - Lx * delta_rad) * sin(model_yaw / 180.0f * PI) + 
	                             (Sensor.DELTA_DIS_Y + Ly * delta_rad) * cos(model_yaw / 180.0f * PI);	
	
}
#endif





