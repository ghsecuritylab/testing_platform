/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      MultiThread.c
  * @brief:     Create all threads.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-07
  * @update:    [2019-01-07][Lei][creat]
  */

/***********************************include***************************************/
#include <board.h>
#include "Led.h"
#include "Sample.h"
#include "KI_test.h"
#include "RtuFrame.h"
#include "TCP_Server.h"

#define TEST_THREAD_NAME                                                "test"
#define TEST_THREAD_PRIORITY                                            (6)
#define TEST_THREAD_STACK_SIZE                                          (2048)
#define TEST_THREAD_TIMESLICE                                           (20)


static void rt_test_thread_entry(void* param)
{
	uint8_t data[18] = {0x02,0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0x00,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
	while(1)
	{
		GenAndSendLongFrame(REMOTE_ADDRESS, UP_CODE, data, 18);
//		PackAndSendData(LOCAL_ADDRESS, UP_CODE, (uint8_t*)"qweytgdfhgrtyuio");
		rt_thread_delay(5000);
	}
}

void TestThread(void)
{
	rt_thread_t Test = NULL;
	Test = rt_thread_create(TEST_THREAD_NAME, 
							rt_test_thread_entry,
							RT_NULL,
							TEST_THREAD_STACK_SIZE,
							TEST_THREAD_PRIORITY,
							TEST_THREAD_TIMESLICE);
	if(NULL != Test)
	{
		rt_thread_startup(Test);
		rt_kprintf("Test thread start\r\n");
	}
}

int rt_multi_thread_start(void)
{
	
    LedThread();
	KItestThread();
	SampleThread();
	TCP_CommunicateThread();
	DataDealThread();
	TestThread();
    return 0;
}
INIT_APP_EXPORT(rt_multi_thread_start);



