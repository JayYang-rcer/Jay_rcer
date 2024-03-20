#ifndef __INCLUDES_H
#define __INCLUDES_H

// Config
#include "defines.h"
#include "FreeRTOSConfig.h"
#include "task_cfg.h"

// 系统文件
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "math.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
// FreeRTOS系统
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "queue.h"


// BSP
#include "timer.h"
#include "pid.h"
#include "can1.h"
#include "m3508.h"
#include "DataScope_DP.h"
#include "ak80.h"
#include "can2.h"
#include "cylinder.h"
#include "tx_handle.h"
#include "gpio.h"

// MODEL
#include "chassis_driver.h"
#include "global_location.h"
#include "pstwo.h"
#include "throwing_institution.h"

// ACTION
#include "move.h"
#include "path.h"
#include "cast.h"
#include "FSM.h"


#endif
