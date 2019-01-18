/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DriverAD7616.h
  * @brief:     AD7616 driver.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-09
  * @update:    [2019-01-09][Lei][creat]
  */

#ifndef _DRIVER_AD7616_H_
#define _DRIVER_AD7616_H_

/*****************************include**********************************/
#include <stdint.h>
#include <stdbool.h>


#define AD7616_DEBUG
/* DEFINES -------------------------------------------------------------------*/
#ifdef  AD7616_DEBUG
#define AD7616_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define AD7616_PRINTF(...)
#endif /* AD7616_DEBUG */


#define AD7616_DATA_ADDR   0x64000000
#define ADC_FIFO_ADDRESS  *((volatile  short *)AD7616_DATA_ADDR)



/**
  * @brief : ADC_SAMPLE_LEN
  * @param : 将采样长度从128改为2000
  * @return: 
  * @updata: [2019-01-15][zhaochangquan][revise]
  */

//#define ADC_SAMPLE_LEN 2000 //采样长度    采样时间/采样周期



#define ADC_CHANNEL_NUM 32

enum AD7616_State
{
	SAMPLE_READY = 0,			//准备开始采样
	SAMPLE_GOING = 1,			//正在进行采样
	SAMPLE_COMPLETE = 2,		//采样完成，数据还未进行处理
};


extern int16_t g_SampleAdcData[ADC_CHANNEL_NUM];
extern uint16_t g_SampleIndex;
extern enum AD7616_State g_SampeState;


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
extern int rt_hw_adc_init(void);



void AD7616Init(void);
void StartADCPWM(void);
void StopADCPWM(void);
int32_t readChanelData(uint16_t chanelNum, uint16_t adcIndex);


#endif

