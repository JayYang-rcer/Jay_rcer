#ifndef __THROWING_H
#define __THROWING_H

#include "sys.h" 

// ×ó/ÓÒÉä¼ý»ú¹¹ID
#define LEFT_TI	 0x00
#define RIGHT_TI 0x01
#define RESET_TI 0x02
#define SET_TI	 0x03

#define SET					0x01
#define UNSET				0x00

#define YAW_MAX			90.0f
#define YAW_MIN			-90.0f

#define PITCH_MAX		90.0f
#define PITCH_MIN		-90.0f

#define RPM_MAX			1340
#define RPM_MIN			940


typedef struct
{	
	float yaw_des;
	float pitch_des;
	int rpm_des;
	u8 is_set;
	
}CTRL_DES_DEF;

extern CTRL_DES_DEF CTRL_DES;
extern const CTRL_DES_DEF CTRL_RESET;

void Throwing_Institution_Control(u8 ID, float yaw_des, float pitch_des, int rpm_des, u8 is_set);

#endif
