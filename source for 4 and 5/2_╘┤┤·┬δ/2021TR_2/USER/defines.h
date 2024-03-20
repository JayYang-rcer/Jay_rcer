#ifndef __DEFINES_H
#define __DEFINES_H

#define PI 								 3.14159265358979f
#define COS45              0.70710678f
#define SIN45              0.70710678f

#define ABS(x)      ((x)>0? (x):(-(x)))

//条件编译
#define USE_RUNTIMESTATS configGENERATE_RUN_TIME_STATS
#define USE_DATASCOPE		 		0
#define USE_MIT_DRIVER			1		// 是否上mit驱动器
#define IS_LEFT_OR_RIGHT		0 	// 0为左射箭机构，1为右射箭机构


#define LEFT_ID		0x00
#define RIGHT_ID	0x01
#define RESET_ID	0x02
#define SET_ID		0x03

#if IS_LEFT_OR_RIGHT
#define CUR_ID		RIGHT_ID
#else
#define CUR_ID		LEFT_ID
#endif

#define SET					0x01
#define UNSET				0x00

#define YAW_MAX			90.0f
#define YAW_MIN			-90.0f

#define PITCH_MAX		90.0f
#define PITCH_MIN		-90.0f

#define RPM_MAX			1340
#define RPM_MIN			940

#endif
