/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      Sample.c
  * @brief:     �����������.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-09
  * @update:    [2019-01-09][Lei][creat]
  */

/***********************************include***************************************/
#include <rtthread.h>
#include <rtconfig.h>
#include "stdbool.h"
#include "Sample.h"
#include "DriverAD7616.h"
#include "DriverKI.h"


/* ����������� */
static struct rt_thread rt_thread_sample;
static rt_uint8_t rt_thread_sample_stack[SAMPLE_THREAD_STACK_SIZE];


static void rt_sample_thread_entry(void* param);

/**************************function***********************************/

/**
  * @brief : sample thread
  * @param : void
  * @return: void 
  * @updata: [2019-01-09][Lei][create]
  */
void SampleThread(void)
{
	rt_err_t result = RT_EOK;
	result = rt_thread_init(&rt_thread_sample,
                            SAMPLE_THREAD_NAME, 
                            rt_sample_thread_entry,
                            RT_NULL,
                            rt_thread_sample_stack,
                            SAMPLE_THREAD_STACK_SIZE,
                            SAMPLE_THREAD_PRIORITY,
                            SAMPLE_THREAD_TIMESLICE);
	if(result == RT_EOK)
	{
		rt_thread_startup(&rt_thread_sample);
		rt_kprintf("Sample thread start\r\n");
	}
}

/**
  * @brief : sample thread entry
  * @param : void*
  * @return: void 
  * @updata: [2019-01-09][Lei][create]
             [2019-01-15][zhaochangquan][revise]
  */




static void rt_sample_thread_entry(void* param)
{
	AD7616Init();
	KIInit();
	rt_thread_delay(5000);
	int i;
	
	while(1)
	{
		
		while((Run_Stop) == 0)      //Ϊ1ֹͣʵ��
		{
			if(FHZ_STATE)           //�ֺ�բ״ָ̬ʾ��1Ϊ��բ״̬��0Ϊ��բ״̬
			{
				FZ_ON;     //�����բָ��
				FZ_ON;
				rt_thread_delay(10);
				FZ_OFF;    //ȡ�������բָ��
			}
			else
			{
				HZ_ON;     //�����բָ��
				HZ_ON;
				rt_thread_delay(10);
				HZ_OFF;    //ȡ�������բָ��
			}
			
			
			if(SAMPLE_READY == g_SampeState)
			{
				StartADCPWM();
			}
			
			if(SAMPLE_COMPLETE == g_SampeState)
			{
				for(uint32_t j = 0; j < (ADC_CHANNEL_NUM-2); j++)
				{
					rt_kprintf("g_SampleAdcData[%d][0] = %d.\r\n", j, g_SampleAdcData[j][0]);
				}
				g_SampeState = SAMPLE_READY;
			}
				
		}
		

		rt_kprintf("Wait for sample!!!\r\n");
		rt_thread_delay(3000);
	}
}

