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




#include "stdint.h"
#include "stm32f4xx_hal.h"
/**************************DEFINES***********************************/

#define LOW 0
#define HIGH 1

#define SingleExecution  KI_Get()&0x0001          //单次试验控制
#define LoopExecution  (KI_Get()&0x0002)>>1       //多次试验控制
#define Run_Stop  (KI_Get()&0x0004)>>2       //开始试验（输入闭合触电开始，输入断开触电停止试验）
#define IN4  (KI_Get()&0x0008)>>3       
#define IN5  (KI_Get()&0x0010)>>4
#define IN6  (KI_Get()&0x0020)>>5
#define IN7  (KI_Get()&0x0040)>>6
#define FHZ_STATE  (KI_Get()&0x0080)>>7


#define HZ_ON  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET)    //输出合闸指令
#define HZ_OFF  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET)   //停止输出合闸指令
#define FZ_ON  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, GPIO_PIN_SET)       //输出分闸指令
#define FZ_OFF  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, GPIO_PIN_RESET)     //停止输出分闸指令

void KIInit(void);	
uint16_t KI_Get(void);

//extern unsigned char LowBuffer[];   //缓存前八路开入输入   
//extern unsigned char HighBuffer[];  //缓存后八路开入输入



#endif

