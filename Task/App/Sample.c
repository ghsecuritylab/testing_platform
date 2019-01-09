/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      Sample.c
  * @brief:     采样任务相关.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-09
  * @update:    [2019-01-09][Lei][creat]
  */

/***********************************include***************************************/
#include <rtthread.h>
#include <rtconfig.h>
#include "Sample.h"

/* 采样任务相关 */
static struct rt_thread rt_thread_sample;
static rt_uint8_t rt_thread_sample_stack[SAMPLE_THREAD_STACK_SIZE];


static void rt_sample_thread_entry(void* param);

/**************************function***********************************/

/**
  * @brief : sample thread
  * @param : void
  * @return: void 
  * @updata: [2019-01-09][Lei][create]
  */
void SampleThread(void)
{
	rt_err_t result = RT_EOK;
	result = rt_thread_init(&rt_thread_sample,
                            SAMPLE_THREAD_NAME, 
                            rt_sample_thread_entry,
                            RT_NULL,
                            rt_thread_sample_stack,
                            SAMPLE_THREAD_STACK_SIZE,
                            SAMPLE_THREAD_PRIORITY,
                            SAMPLE_THREAD_TIMESLICE);
	if(result == RT_EOK)
	{
		rt_thread_startup(&rt_thread_sample);
		rt_kprintf("Sample thread start\r\n");
	}
}

/**
  * @brief : sample thread entry
  * @param : void*
  * @return: void 
  * @updata: [2019-01-09][Lei][create]
  */
static void rt_sample_thread_entry(void* param)
{
	
	while(1)
	{
		
		rt_thread_delay(1000);
	}
}

