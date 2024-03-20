#ifndef __INFORMATION_H
#define __INFORMATION_H

#include "stm32f4xx.h"

//做时间分频用
#define RATE_5_HZ	    5
#define RATE_10_HZ		10
#define RATE_20_HZ		20
#define RATE_25_HZ		25
#define RATE_50_HZ		50
#define RATE_100_HZ		100
#define RATE_200_HZ 	200
#define RATE_250_HZ 	250
#define RATE_500_HZ 	500
#define RATE_1000_HZ 	1000

/*传感器读取更新频率*/
#define GPIO_UPDATE_RATE		      RATE_1000_HZ
#define Laser_UPDATE_RATE		     	RATE_10_HZ
#define Encoder_UPDATE_RATE		    RATE_1000_HZ
#define Gyro_UPDATE_RATE		      RATE_1000_HZ
#define GL_UPDATE_RATE		        RATE_1000_HZ
#define Motor_UPDATE_RATE         RATE_50_HZ
#define velocity_UPDATE_RATE		  RATE_100_HZ

#define RATE_DO_EXECUTE(RATE_HZ, TICK) ((TICK % (RATE_1000_HZ / RATE_HZ)) == 0)
#define Laser_Num      3

typedef struct INFORMATION
{
	u8  GPIO_KEY;
	u16  GPIO_limit;
	float M3508_angle;
	float laser_Distance[Laser_Num];
	float Gyro ;
	int CNT_X ;
	int CNT_Y ;
	float DELTA_DIS_X;
	float DELTA_DIS_Y;
	float DIS_X;
	float DIS_Y;
	float real_Vx;
	float real_Vy;
} INFORMATION;

extern INFORMATION Sensor;

float DT_LV(int queue[] ,u8 num , int count); //速推均值滤波
void Sensor_Update(void *pdata);
void Read_GPIO(void);
void Read_encoder(void);
void Read_Gyro(void);
void Read_V_real(void);
u8 Get_KEY(u16 name);
u8 Get_limit(u16 name);

#endif
