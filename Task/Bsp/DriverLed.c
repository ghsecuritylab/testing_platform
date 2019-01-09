/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DriverLed.c
  * @brief:     Led driver.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-08
  * @update:    [2019-01-08][Lei][creat]
  */

/*****************************include**********************************/
#include <rtthread.h>
#include "stm32f4xx_hal.h"
#include "DriverLed.h"


/***********************static*****************************/
struct IO_handle RunLedHandle;		//运行指示灯的句柄


/**************************function***********************************/

/**
  * @brief : led initialize
  * @param : void
  * @return: void 
  * @updata: [2019-01-08][Lei][create]
  */  
void InitLed(void)
{
	RunLedHandle.GPIOPin = GPIO_PIN_0;
	RunLedHandle.GPIOx = GPIOB;
	RunLedHandle.Write = GPIO_WriteStatus;
	RunLedHandle.Read = NULL;
}

/**
  * @brief : led on and off
  * @param : ledHandle led灯句柄
  * @param : ledState 灯的状态：LED_ON LED_OFF
  * @return: void 
  * @updata: [2019-01-08][Lei][create]
  */
void LedOnAndOff(struct IO_handle ledHandle, uint32_t ledState)
{
	ledHandle.Write(ledHandle, ledState);
}




