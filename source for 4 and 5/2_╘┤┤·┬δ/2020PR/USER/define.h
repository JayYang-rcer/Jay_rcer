#ifndef __DEFINE_H
#define __DEFINE_H

#define PI					3.14159265358979f

#define ANGLE2RAD(x) (x/180.0f*PI)  //角度转弧度
#define RAD2ANGLE(x) (x/PI*180.0f)  //弧度转角度

//绝对值
#define ABS(x)      ((x)>0? (x):(-(x)))

//GPIO定义
//开关
#define START_KEY             0x01       //启动开关      //E8
#define RESTART_TO_1_KEY      0x02       //遥控开关      //E9
#define RESTART_TO_2_KEY      0x04       //去踢球开关   //E10
#define RESTART_TO_3_KEY  		0x08       //回来开关     //E11
#define KICK_START_KEY  		  0x10       //踢球开关     //E12
#define KICK_KEY              0x20       //踢球气缸     //E13
#define TO_5th_ball           0x40                        //E14
 
#define ARM_RESET_LIMIT       0x0020       //手臂复位
#define ARM_ARRIVE_LIMIT      0x0010       //手臂到达









#endif
