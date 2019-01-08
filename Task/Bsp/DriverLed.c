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
#include "DriverIO.h"


/***********************static*****************************/
static struct IO_handle RunLedHandle;


/**************************function***********************************/

/**
  * @brief : led GPIO initialize
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
	
    Init_GPIO();
	
}

/**
  * @brief : led on
  * @param : void
  * @return: void 
  * @updata: [2019-01-08][Lei][create]
  */
void LedOn(void)
{
	RunLedHandle.Write(RunLedHandle, LED_ON);
}

/**
  * @brief : led off
  * @param : void
  * @return: void 
  * @updata: [2019-01-08][Lei][create]
  */
void LedOff(void)
{
	RunLedHandle.Write(RunLedHandle, LED_OFF);
}


