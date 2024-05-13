/**
 * @file exins.h
 * @brief 模拟I2C从机操作需要保函的头文件
 *
 * @version 1.0.0
 * @date 2024-04-28
 *
 * @copyright Copyright (c) 2024, wujingsheng 
 * This source code is released under the MIT License.
 *
 * @author wujingsheng
 *
 * @brief 本文件实现了,添加与硬件平台相关的头文件.
 * 
 */

#ifndef __EXINS_H__
#define __EXINS_H__

#include <stdio.h>
#include <string.h>

/*************************RF8018XH 需要添加的头文件***********************/
#include "gap_api.h"
#include "gatt_api.h"
#include "os_timer.h"
#include "os_mem.h"
#include "sys_utils.h"
#include "User_Uart.h"
#include "button.h"
#include "jump_table.h"

#include "user_task.h"
#include "prf_server.h"
#include "prf_client.h"

#include "driver_timer.h"
#include "driver_plf.h"
#include "driver_system.h"
#include "driver_pmu.h"
#include "driver_uart.h"
#include "driver_rtc.h"
#include "driver_efuse.h"
#include "driver_flash.h"
#include "driver_gpio.h"
#include "driver_exti.h"
#include "flash_usage_config.h"

/* FreeRTOS headers */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "event_groups.h"
#include "os_timer.h"
/************************************************************************/


/****************与项目相关的头文件*****************/
#include "user_systemconfig.h"
#include "firmware_version2adv.h"



#ifndef defErrOK
#define defErrOK 0
#endif




#endif 
