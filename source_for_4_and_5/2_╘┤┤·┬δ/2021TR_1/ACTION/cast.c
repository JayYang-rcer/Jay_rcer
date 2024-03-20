#include "includes.h"


float CAST_MOTOR_TARGET_RPM = 0 ;    // 抬升电机目标速度


// 射箭蓄力
MOTOR_VELOCITY_PLANNING cast_motor_motion_0m = {0,    3.0, 100, 2800, 100, 0.3, 0.3};				// 复位
MOTOR_VELOCITY_PLANNING cast_motor_motion_4m = {0, -440.0, 600, 2300, 100, 0.2, 0.2};				// 射4m (end = -470.0
MOTOR_VELOCITY_PLANNING cast_motor_motion_6m = {-440.0, -550.0, 600,  1500, 100, 0.3, 0.3};	// 射6m



// 电机RPM规划
int motor_plan_first_time_flag = 1;
int MotorRPMPlan(MOTOR_VELOCITY_PLANNING motion, float pos, int16_t *motor_target_rpm)
{
	static float Ssu;   //总路程
	static float Sac;   //加速路程
	static float Sde;   //减速路程
	static float Sco;   //匀速路程
	static float Aac;   //加速加速度
	static float Ade;   //减速加速度
	static float S;     //当前路程
	
	//到达目标点
	if(motion.Pend < motion.Pstart && pos < motion.Pend)
	{
		motor_plan_first_time_flag = 1;
		return 1;
	}
	else if(motion.Pend > motion.Pstart && pos > motion.Pend)
	{
		motor_plan_first_time_flag = 1;
		return 1;
	}
	
	// 如果所配数据有误，则不执行速度规划
	if((motion.Rac > 1) || (motion.Rac < 0) ||
		 (motion.Rde > 1) || (motion.Rde < 0) ||
		 (motion.Vmax < motion.Vstart) )
	{
		*motor_target_rpm = 0;  // 令电机不动
		return -1;
	}
	
	// 匀速模式
	if(motion.Pstart == motion.Pend)
	{
		*motor_target_rpm = motion.Vstart * motion.Vmax;
		return 0;
	}
	
	// 第一次调用，计算一些变量
	if(motor_plan_first_time_flag)
	{
		motor_plan_first_time_flag = 0;
		Ssu = ABS(motion.Pend - motion.Pstart);    
		Sac = Ssu * motion.Rac;
		Sde = Ssu * motion.Rde;
		Sco = Ssu - Sac - Sde;
		Aac = (motion.Vmax * motion.Vmax - motion.Vstart * motion.Vstart) / (2.0f * Sac);
		Ade = (motion.Vend * motion.Vend -   motion.Vmax *   motion.Vmax) / (2.0f * Sde);
	}
	
	// 过滤异常情况
	if(((motion.Pend > motion.Pstart) && (pos < motion.Pstart)) ||
		 ((motion.Pend < motion.Pstart) && (pos > motion.Pstart)))
	{
		*motor_target_rpm = motion.Vstart;
	}
	else if(((motion.Pend > motion.Pstart) && (pos > motion.Pend)) ||
		      ((motion.Pend < motion.Pstart) && (pos < motion.Pend)))
	{
		*motor_target_rpm = motion.Vend;
	}
	else
	{
		S = ABS(pos - motion.Pstart);
		
		// 规划RPM
		if     (S < Sac)       *motor_target_rpm = sqrt(2.0f * Aac * S + motion.Vstart * motion.Vstart);         // 加速阶段
		else if(S < (Sac+Sco)) *motor_target_rpm = motion.Vmax;                                                  // 匀速阶段
		else                   *motor_target_rpm = sqrt(motion.Vend * motion.Vend - 2.0f * Ade * ABS(Ssu - S));  // 减速阶段
	}
	
	// 分配合适的正负号
	if(motion.Pend < motion.Pstart) *motor_target_rpm = -*motor_target_rpm;
	return 0;
}





