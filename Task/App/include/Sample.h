/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      Sample.h
  * @brief:     采样任务相关.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-09
  * @update:    [2019-01-09][Lei][creat]
  */


#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#define SAMPLE_THREAD_NAME                                                "sample"
#define SAMPLE_THREAD_PRIORITY                                            (6)
#define SAMPLE_THREAD_STACK_SIZE                                          (2048)
#define SAMPLE_THREAD_TIMESLICE                                           (20)

void SampleThread(void);

#endif

