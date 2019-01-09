/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      Led.h
  * @brief:     Led应用相关.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-07
  * @update:    [2019-01-07][Lei][creat]
  */

#ifndef _LED_H_
#define _LED_H_

#define LED_THREAD_NAME                                                "led"
#define LED_THREAD_PRIORITY                                            (10)
#define LED_THREAD_STACK_SIZE                                          (256)
#define LED_THREAD_TIMESLICE                                           (20)

void LedThread(void);

#endif
