/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      LedThread.c
  * @brief:     Led应用相关.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-07
  * @update:    [2019-01-07][Lei][creat]
  */

/***********************************include***************************************/
#include <rtthread.h>
#include <rtconfig.h>
#include "Led.h"
#include "DriverLed.h"

/* Led灯任务相关 */
static struct rt_thread rt_thread_led;
static rt_uint8_t rt_thread_led_stack[LED_THREAD_STACK_SIZE];


static void rt_led_thread_entry(void* param);

/**************************function***********************************/

/**
  * @brief : led thread
  * @param : void
  * @return: void 
  * @updata: [2019-01-08][Lei][create]
  */
void LedThread(void)
{
	rt_err_t result = RT_EOK;
	result = rt_thread_init(&rt_thread_led,
                            LED_THREAD_NAME, 
                            rt_led_thread_entry,
                            RT_NULL,
                            rt_thread_led_stack,
                            LED_THREAD_STACK_SIZE,
                            LED_THREAD_PRIORITY,
                            LED_THREAD_TIMESLICE);
	if(result == RT_EOK)
	{
		rt_thread_startup(&rt_thread_led);
		rt_kprintf("Led thread start\r\n");
	}
}

/**
  * @brief : led thread entry
  * @param : void*
  * @return: void 
  * @updata: [2019-01-08][Lei][create]
  */
static void rt_led_thread_entry(void* param)
{
	static uint32_t runLedState = LED_OFF;
	InitLed();
	while(1)
	{
		if(LED_OFF == runLedState)
		{
			LedOn();
			runLedState = LED_ON;
		}
		else
		{
			LedOff();
			runLedState = LED_OFF;
		}
		rt_thread_delay(1000);
	}
	
}



