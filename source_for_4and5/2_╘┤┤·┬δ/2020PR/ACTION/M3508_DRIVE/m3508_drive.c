#include "includes.h"

ARM_VELOCITY_PLANNING *ARM_NOW_MOTION = NULL;  // 指向摇臂当前动作的指针
ARM_VELOCITY_PLANNING ARM_HOLD_MOTION  = {0, 0, 0,   0, 0, 0, 0};  // 不变
ARM_VELOCITY_PLANNING ARM_RESET_MOTION = {0, 120, 1000, 1200, 300, 0.2, 0.3};  // 要做规划
ARM_VELOCITY_PLANNING ARM_SHOOT_MOTION = {ARM_INIT_POS, ARM_SHOOT_POS, 500, 800, 0, 0.2, 0.3};  // 要做规划
ARM_VELOCITY_PLANNING ARM_SHOOT_MOTION2 = {ARM_INIT_POS, ARM_SHOOT_POS, 1000, 1500, 0, 0.2, 0.3};  // 要做规划
ARM_VELOCITY_PLANNING ARM_SHOOT_2_MOTION = {ARM_INIT_POS, -60, 1000, 2000, 1000, 0.2, 0.3};  // 要做规划


void Motor_motion_control(void *pdata)
{
	while(1)
	{

		// 夹球手臂动作控制
		M3508_arm_motor_motion_control();
		M2006_turnplate_motion_control();
		
	  m2006_send_motor_currents();
    m3508_send_motor_currents();
	
		KICK_control();

		OSTimeDly(5);
	}
}


// 夹球手臂动作控制
void M3508_arm_motor_motion_control(void)
{
	// 规划速度
	ad_plan_arm_motor_RPM(*ARM_NOW_MOTION, M3508_ARM_MOTOR_REAL_INFO.REAL_ANGLE);
	
	// 利用PID跟踪目标速度
	PID_incremental_PID_calculation(&M3508_ARM_MOTOR_PID_RPM, M3508_ARM_MOTOR_REAL_INFO.RPM, ARM_TURN_MOTOR_TARGET_RPM);
	
	// 设置电流
	M3508_ARM_MOTOR_REAL_INFO.TARGET_CURRENT = M3508_ARM_MOTOR_PID_RPM.output;
}

// 上转盘动作控制
void GM6020_turnplate_motion_control(void)   
{

	 // 利用PID跟踪目标速度
	 PID_incremental_PID_calculation(&GM6020_TURN_MOTOR_PID_RPM, GM6020_TURN_MOTOR_REAL_INFO.RPM, TURN_MOTOR_TARGET_RPM);
	 // 设置电流
	 GM6020_TURN_MOTOR_REAL_INFO.TARGET_CURRENT = GM6020_TURN_MOTOR_PID_RPM.output;
}

// 上转盘动作控制
void M2006_turnplate_motion_control(void)   
{

	 // 利用PID跟踪目标速度
	 PID_incremental_PID_calculation(&M2006_TURN_MOTOR_PID_RPM, M2006_TURN_MOTOR_REAL_INFO.RPM, TURN_MOTOR_TARGET_RPM);
	 // 设置电流
	 M2006_TURN_MOTOR_REAL_INFO.TARGET_CURRENT = M2006_TURN_MOTOR_PID_RPM.output;
}


// 规划夹爪机械臂电机应有的RPM
void ad_plan_arm_motor_RPM(ARM_VELOCITY_PLANNING motion, float pos)
{
	float Ssu;   //总路程
	float Sac;   //加速路程
	float Sde;   //减速路程
	float Sco;   //匀速路程
	float Aac;   //加速加速度
	float Ade;   //减速加速度
	float S;     //当前路程
	
	// 如果所配数据有误，则不执行速度规划
	if((motion.Rac > 1) || (motion.Rac < 0) ||
		 (motion.Rde > 1) || (motion.Rde < 0) ||
		 (motion.Vmax < motion.Vstart) )
	{
		ARM_TURN_MOTOR_TARGET_RPM = 0;  // 令夹爪不运动
		return;
	}
	
	// 匀速模式
	if(motion.Pstart == motion.Pend)
	{
		ARM_TURN_MOTOR_TARGET_RPM = motion.Vstart * motion.Vmax;
		return;
	}
	
	// 计算一些变量
	Ssu = ABS(motion.Pend - motion.Pstart);    
	Sac = Ssu * motion.Rac;
	Sde = Ssu * motion.Rde;
	Sco = Ssu - Sac - Sde;
	Aac = (motion.Vmax * motion.Vmax - motion.Vstart * motion.Vstart) / (2.0f * Sac);
	Ade = (motion.Vend * motion.Vend -   motion.Vmax *   motion.Vmax) / (2.0f * Sde);
	
	// 过滤异常情况
	if(((motion.Pend > motion.Pstart) && (pos < motion.Pstart)) ||
		 ((motion.Pend < motion.Pstart) && (pos > motion.Pstart)))
	{
		ARM_TURN_MOTOR_TARGET_RPM = motion.Vstart;
	}
	else if(((motion.Pend > motion.Pstart) && (pos > motion.Pend)) ||
		      ((motion.Pend < motion.Pstart) && (pos < motion.Pend)))
	{
		ARM_TURN_MOTOR_TARGET_RPM = motion.Vend;
	}
	else
	{
		S = ABS(pos - motion.Pstart);
		
		// 规划RPM
		if     (S < Sac)       ARM_TURN_MOTOR_TARGET_RPM = sqrt(2.0f * Aac * S + motion.Vstart * motion.Vstart);               // 加速阶段
		else if(S < (Sac+Sco)) ARM_TURN_MOTOR_TARGET_RPM = motion.Vmax;                                                        // 匀速阶段
		else                   ARM_TURN_MOTOR_TARGET_RPM = sqrt(motion.Vend * motion.Vend - 2.0f * Ade * ABS(Ssu - S));  // 减速阶段
	}
	
	// 分配合适的正负号
	if(motion.Pend < motion.Pstart) ARM_TURN_MOTOR_TARGET_RPM = -ARM_TURN_MOTOR_TARGET_RPM;
}

