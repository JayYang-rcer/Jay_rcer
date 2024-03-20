#include <includes.h>

/*读取传感器信息，并作初步处理
*/
//递推滤波函数
float DT_LV ( int queue[] ,u8 num , int count )
{
	float all = 0 ; 
	u8 i,j;
  //数组递推	
	for(i=0; i<num-1; i++)                             
	{   
	  queue[i]=queue[i+1];  
	}
	//末尾赋值
	queue[num-1] = count;    
	//求和
	for(j=0; j<num; j++ )
	{
	 all += queue[j] ;
	}
	
  return all/num;  
}

INFORMATION Sensor ;
//读取GPIO状态信息
//限位开关
//光电开关
//色标传感器
void Read_GPIO()
{
	//读取电平
	Sensor.GPIO_KEY = GET_GPIO_KEY();
	Sensor.GPIO_limit = Get_GPIO_limit();
}

//读取编码器参数
//全场定位编码器
//其他电机
void Read_encoder()
{
	//建立编码器数组
//	static int Encoder[2][10] = {0};
	
	// 读取定时器CNT寄存器里面的值
	// 朝外运动，值应该为正，根据这个调整正负号
//	Sensor.CNT_X =  DT_LV( Encoder[0] ,10,-encoder_get_encoder_CNT(4));  // 接顺时针首轮子
//	Sensor.CNT_Y =  DT_LV( Encoder[1] ,10,-encoder_get_encoder_CNT(8));  // 接顺时针尾轮子
	Sensor.CNT_X = encoder_get_encoder_CNT(4);
  Sensor.CNT_Y = encoder_get_encoder_CNT(8);
}

void Read_Gyro()
{
		// 直接获取偏航角，并进行简单处理（偏航角度值全是逆时针为正，顺时针为负）
	
	  robot.angle = (float)GYRO_ANGLE.Angle[2] / 32768 * 180 - SELF_GL_YAW_INIT_VALUE + OFFSET_YAW ;  /*WAITING_TEST*/
	
	  // 限制范围(-180， 180]
	  if(robot.angle > 180)        robot.angle = -180 + (robot.angle - 180);
	  else if(robot.angle <= -180) robot.angle =  180 + (robot.angle + 180);	
}

void Read_V_real()
{
	static float last_x = 0;
	static float last_y = 0;
	
	Sensor.real_Vx = (ROBOT_REAL_POS_DATA.X_POS - last_x)/5;
	Sensor.real_Vy = (ROBOT_REAL_POS_DATA.Y_POS - last_y)/5;
	
	last_x = ROBOT_REAL_POS_DATA.X_POS;
	last_y = ROBOT_REAL_POS_DATA.Y_POS;
}

u8 Get_KEY(u16 name)
{
  return Sensor.GPIO_KEY & name ;
}

u8 Get_limit(u16 name)
{
	return Sensor.GPIO_limit & name ;
}

//传感器更新函数  
void Sensor_Update(void *pdata)
{
    u32 ticks = 0;
	  u32 lastWakeTime = Get_OSTime();
	  while(1)
	  {
			OSTimeDlyHMSM(0,0,0,1);
			#if Self_Location	  
		  if (RATE_DO_EXECUTE(Encoder_UPDATE_RATE, ticks))
		  {
			 Read_encoder();
		  }

			if (RATE_DO_EXECUTE(GL_UPDATE_RATE, ticks))
		  {
       Update_self_gl_position();
		  }
      #endif
			if (RATE_DO_EXECUTE(GPIO_UPDATE_RATE, ticks))
		  {
			 Read_GPIO();
	    }	
			if (RATE_DO_EXECUTE(Gyro_UPDATE_RATE, ticks))
		  {
//			 Read_Gyro();
		  }
		  if (RATE_DO_EXECUTE(Motor_UPDATE_RATE, ticks))
		  {
       romote_action_task();
		  }
			if (RATE_DO_EXECUTE(Motor_UPDATE_RATE, ticks))
		  {
       Read_V_real();				
		  }
			
		  ticks++; 

	  }
}
