/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DriverKI.h
  * @brief:     KI driver.
  * @version:   V1.0.0 
  * @author:    ZhaoChanqquan
  * @date:      2019-01-09
  * @update:    [2019-01-09][ZhaoChangquan][creat]
  */
 
#ifndef _DRIVER_KI_H_
#define _DRIVER_KI_H_

/**************************DEFINES***********************************/

#define LOW 0
#define HIGH 1

void KIInit(void);	
unsigned int KI_Get(unsigned int num);

//extern unsigned char LowBuffer[];   //缓存前八路开入输入   
//extern unsigned char HighBuffer[];  //缓存后八路开入输入



#endif

