/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DriverLed.h
  * @brief:     Led driver.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-08
  * @update:    [2019-01-08][Lei][creat]
  */
 
#ifndef _DRIVER_LED_H_
#define _DRIVER_LED_H_

#include <stdint.h>
#include "DriverIO.h"

/**************************DEFINES***********************************/
#ifdef  LED_DEBUG
#define LED_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define LED_PRINTF(...)
#endif /* LED_DEBUG */

#define LED_ON		0
#define LED_OFF		1


extern struct IO_handle RunLedHandle;

void InitLed(void);
void LedOnAndOff(struct IO_handle ledHandle, uint32_t ledState);

#endif

