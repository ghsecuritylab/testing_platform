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

int rt_multi_thread_start(void)
{
	
    LedThread();
	KItestThread();
	SampleThread();
	
    return 0;
}
INIT_APP_EXPORT(rt_multi_thread_start);

