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



#define AD7616_DEBUG
/* DEFINES -------------------------------------------------------------------*/
#ifdef  AD7616_DEBUG
#define AD7616_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define AD7616_PRINTF(...)
#endif /* AD7616_DEBUG */


#define AD7616_DATA_ADDR   0x64000000
#define ADC_FIFO_ADDRESS  *((volatile  short *)AD7616_DATA_ADDR)


#define ADC_SAMPLE_LEN 128 //采样长度
#define ADC_CHANNEL_NUM 32

#define g_SampleNum 1500

void AD7616Init(void);
void StartADCPWM(void);
void StopADCPWM(void);
int32_t readChanelData(uint16_t chanelNum, uint16_t adcIndex);

extern int16_t g_SampleAdcData[ADC_CHANNEL_NUM][ADC_SAMPLE_LEN ] ;
extern uint16_t g_SampleIndex;
extern int16_t g_Sample[4][g_SampleNum];


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
extern int rt_hw_adc_init(void);



#endif

