/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TCP_Server.h
  * @brief:     TCP服务端相关任务.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-14
  * @update:    [2019-01-14][Lei][creat]
  */


#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <stdint.h>


#define TCP_COMMUNICATE_THREAD_NAME                                                "tcp"
#define TCP_COMMUNICATE_THREAD_PRIORITY                                            (6)
#define TCP_COMMUNICATE_THREAD_STACK_SIZE                                          (4096)
#define TCP_COMMUNICATE_THREAD_TIMESLICE                                           (20)



void TCP_CommunicateThread(void);



#endif

