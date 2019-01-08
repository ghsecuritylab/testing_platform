/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DriverIO.h
  * @brief:     GPIO driver.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-08
  * @update:    [2019-01-08][Lei][creat]
  */
 
#ifndef _DRIVER_IO_H_
#define _DRIVER_IO_H_

#include "stdint.h"
#include "stm32f4xx_hal.h"


struct IO_handle
{
	uint16_t GPIOPin;
	GPIO_TypeDef* GPIOx;
	void (*Write)(struct IO_handle, int);
	int (*Read)(struct IO_handle);
};


void Init_GPIO(void);
void GPIO_WriteStatus(struct IO_handle handle, int state);
int GPIO_ReadStatus(struct IO_handle handle);


#endif
