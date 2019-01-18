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

#define SingleExecution  KI_Get()&0x0001          //�����������
#define LoopExecution  (KI_Get()&0x0002)>>1       //����������
#define Run_Stop  (KI_Get()&0x0004)>>2       //��ʼ���飨����պϴ��翪ʼ������Ͽ�����ֹͣ���飩
#define IN4  (KI_Get()&0x0008)>>3       
#define IN5  (KI_Get()&0x0010)>>4
#define IN6  (KI_Get()&0x0020)>>5
#define IN7  (KI_Get()&0x0040)>>6
#define FHZ_STATE  (KI_Get()&0x0080)>>7


#define HZ_ON  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET)    //�����բָ��
#define HZ_OFF  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET)   //ֹͣ�����բָ��
#define FZ_ON  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, GPIO_PIN_SET)       //�����բָ��
#define FZ_OFF  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, GPIO_PIN_RESET)     //ֹͣ�����բָ��

void KIInit(void);	
uint16_t KI_Get(void);

//extern unsigned char LowBuffer[];   //����ǰ��·��������   
//extern unsigned char HighBuffer[];  //������·��������



#endif

