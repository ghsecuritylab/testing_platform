/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      KIThread.c
  * @brief:     KI相关程序测试
  * @version:   V1.0.0 
  * @author:    ZhaoChangquan
  * @date:      2019-01-10
  * @update:    [2019-01-10][ZhaoChangquan][creat]
  */

/***********************************include***************************************/
#include <rtthread.h>
#include <rtconfig.h>
#include "KI_test.h"
#include "DriverKI.h"

#include "stdint.h"
#include "stm32f4xx_hal.h"


/* KI测试任务相关 */

static struct rt_thread rt_thread_KItest;
static rt_uint8_t rt_thread_KItest_stack[KItest_THREAD_STACK_SIZE];


static void rt_KItest_thread_entry(void* param);

/**************************function***********************************/

/**
  * @brief : KItest THread
  * @param : void
  * @return: void 
  * @updata: [2019-01-10][ZhaoChangquan][create]
  */
void KItestThread(void)
{
	rt_err_t result = RT_EOK;
	result = rt_thread_init(&rt_thread_KItest,
                            KItest_THREAD_NAME, 
                            rt_KItest_thread_entry,
                            RT_NULL,
                            rt_thread_KItest_stack,
                            KItest_THREAD_STACK_SIZE,
                            KItest_THREAD_PRIORITY,
                            KItest_THREAD_TIMESLICE);
	if(result == RT_EOK)
	{
		rt_thread_startup(&rt_thread_KItest);
		rt_kprintf("KItest thread start\r\n");
	}
}

/**
  * @brief : KItest thread entry
  * @param : void*
  * @return: void 
  * @updata: [2019-01-10][ZhaoChangquan][create]
  */
static void rt_KItest_thread_entry(void* param)
{
//	KIInit();

	int i=0;
	int c;
	while(1)
	{
		if(i<256)
		{
			
			
			c=KI_Get();

			
			rt_kprintf("KI_Buffer=%d\r\n",c);

		}
		else
		{
			i=0;
		}
		rt_thread_delay(1000);
		
		
	}
	
	
	
}	
