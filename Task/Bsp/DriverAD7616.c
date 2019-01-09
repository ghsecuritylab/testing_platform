/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DriverAD7616.c
  * @brief:     AD7616 driver.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-09
  * @update:    [2019-01-09][Lei][creat]
  */

/*****************************include**********************************/
#include <math.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "rtconfig.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
//#include "drv_gpio.h"
//#include "drv_timer.h"
//#include "other_protect.h"
//#include "common_data.h"
//#include ".\MultiThread\multi_thread.h"
//#include "wave_recording.h"
#include "DriverAD7616.h"



/**
* ²ÉÑùÊý¾Ý±£´æ
*/
int16_t g_SampleAdcData[ADC_CHANNEL_NUM][ADC_SAMPLE_LEN];
uint16_t g_SampleIndex;


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static SRAM_HandleTypeDef hsram2;
static TIM_HandleTypeDef TIM5_Handler; // ¶¨Ê±Æ÷5PWM¾ä±ú
static TIM_OC_InitTypeDef TIM5_CH1Handler; // ¶¨Ê±Æ÷5Í¨µÀ1¾ä±ú
static void TIM5_PWM_Init(uint16_t arr, uint16_t pulse, uint16_t psc);
static void AD7616_delay_ns(uint32_t nus);
static void AD7616_FSMC_Init(void);

__STATIC_INLINE void read_ad7616_data(uint16_t adc_index);
__STATIC_INLINE void ADC_BusyInit(void);



/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @description: AD7616 Init.
  * @param[in]  : none
  * @return     : none
  * @updata     : [YYYY-MM-DD] [Change person name][change content]
  */
void AD7616Init(void)
{
    AD7616_FSMC_Init();
    ADC_BusyInit();

    ADC_FIFO_ADDRESS = 0x8000 | ((0x0002 & 0x003f) << 9) | (0x0060 & 0x01ff); //Ð´ÅäÖÃ¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0004 & 0x003f) << 9) | (0x0000 & 0x01ff); //Ð´ÊäÈë·¶Î§¼Ä´æÆ÷A1
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0005 & 0x003f) << 9) | (0x0000 & 0x01ff); //Ð´ÊäÈë·¶Î§¼Ä´æÆ÷A2
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0006 & 0x003f) << 9) | (0x0000 & 0x01ff); //Ð´ÊäÈë·¶Î§¼Ä´æÆ÷B1
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0007 & 0x003f) << 9) | (0x0000 & 0x01ff); //Ð´ÊäÈë·¶Î§¼Ä´æÆ÷B2
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0020 & 0x003f) << 9) | (0x0000 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0021 & 0x003f) << 9) | (0x0011 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0022 & 0x003f) << 9) | (0x0022 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0023 & 0x003f) << 9) | (0x0033 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0024 & 0x003f) << 9) | (0x0044 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0025 & 0x003f) << 9) | (0x0055 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0026 & 0x003f) << 9) | (0x0066 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0027 & 0x003f) << 9) | (0x0077 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0028 & 0x003f) << 9) | (0x0066 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x0029 & 0x003f) << 9) | (0x0055 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x002A & 0x003f) << 9) | (0x0044 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x002B & 0x003f) << 9) | (0x0033 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x002C & 0x003f) << 9) | (0x0022 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x002D & 0x003f) << 9) | (0x0011 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷
    ADC_FIFO_ADDRESS = 0x8000 | ((0x002E & 0x003f) << 9) | (0x0100 & 0x01ff); //ÅäÖÃÐòÁÐÆ÷¶ÑÕ»¼Ä´æÆ÷

    g_SampleIndex = 0;

	TIM5_PWM_Init(3000 - 1, 1000, 3 - 1);	//180MHz/3=60MµÄ¼ÆÊýÆµÂÊ,×Ô¶¯ÖØ×°ÔØÎª6000,PWMÆµÂÊÎª60M/6000,1/(60M/6000)=100us
//    TIM5_PWM_Init(6250 - 1, 1000, 3 - 1);	//180MHz/3=60MµÄ¼ÆÊýÆµÂÊ,×Ô¶¯ÖØ×°ÔØÎª6250,PWMÆµÂÊÎª60M/6250,1/(60M/6250)=104us
//    TIM5_PWM_Init(400 - 1, 200, 9 - 1);	//180M/9=20MHz  20MHz/400=50kHz  1/50kHz=0.02ms=20us


}

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @description: ad7616 delay function.
  * @param[in]  : [nus] us delay.
  * @return     : none
  */
static void AD7616_delay_ns(uint32_t nus)
{
    while (nus--)
    {
        __NOP();
    }
}

/**
  * @description: FSMC Init.
  * @param[in]  : none
  * @return     : none
  * @updata     : [YYYY-MM-DD] [Change person name][change content]
  */
static void AD7616_FSMC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    FMC_NORSRAM_TimingTypeDef Timing;

    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    hsram2.Instance = FMC_NORSRAM_DEVICE;
    hsram2.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram2.Init */
    hsram2.Init.NSBank = FMC_NORSRAM_BANK2;
    hsram2.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_ENABLE;
    hsram2.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;//´æ´¢Æ÷ÀàÐÍ
    hsram2.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;//Êý¾Ý¿í¶È
    hsram2.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram2.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram2.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
    hsram2.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    hsram2.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;//Ð´Ê¹ÄÜ
    hsram2.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    hsram2.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;//À©Õ¹Ä£Ê½Ê¹ÄÜÎ»
    hsram2.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram2.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
    hsram2.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    hsram2.Init.PageSize = FMC_PAGE_SIZE_NONE;

    Timing.AddressSetupTime = 3;
    Timing.AddressHoldTime = 0;
    Timing.DataSetupTime = 5;
    Timing.BusTurnAroundDuration = 1;
    Timing.CLKDivision = 0;
    Timing.DataLatency = 0;
    Timing.AccessMode = FMC_ACCESS_MODE_A;

    HAL_SRAM_Init(&hsram2, &Timing, NULL);
}

/**
  * @Description: Read the sampled data of AD7616
  * @param[in]  : none
  * @return     : none
  * @updata     : [YYYY-MM-DD] [Change person name][change content]
  */
__STATIC_INLINE void read_ad7616_data(uint16_t adc_index)
{

    for (uint16_t adc_chanel = 0; adc_chanel < ADC_CHANNEL_NUM - 2; adc_chanel++)
    {
        g_SampleAdcData[adc_chanel][adc_index] = (ADC_FIFO_ADDRESS);
    }


}



/**
  * @description: ³õÊ¼»¯Òý½Åbusy PH12
  */
__STATIC_INLINE void ADC_BusyInit(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOH_CLK_ENABLE();               //¿ªÆôGPIOHÊ±ÖÓ

    /**/
	GPIO_Initure.Pin=GPIO_PIN_12;               //PH12
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //ÏÂ½µÑØ´¥·¢
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);

    /* EXTI15_10_IRQn interrupt configuration */
    NVIC_SetPriority(EXTI15_10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}


/**
  * @brief: TIM10 PWM³õÊ¼»¯.
  * @param:  arr-.
  * @param:  psc-.
  * @return: void
  * @updata: [2017-08-07][][]
  */
static void TIM5_PWM_Init(uint16_t arr, uint16_t pulse, uint16_t psc)
{
    TIM5_Handler.Instance = TIM5;
    TIM5_Handler.Init.Prescaler = psc;
    TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM5_Handler.Init.Period = arr;
    TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM5_Handler);
    TIM5_CH1Handler.OCMode = TIM_OCMODE_PWM1;
    TIM5_CH1Handler.Pulse = pulse;
    TIM5_CH1Handler.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CH1Handler, TIM_CHANNEL_1);

//    HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_1);
}

/**
* @brief:¿ªÆô´¥·¢
*/
void StartADCPWM(void)
{
    HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_1);
}
/**
* @brief:Í£Ö¹´¥·¢
*/
void StopADCPWM(void)
{
    HAL_TIM_PWM_Stop(&TIM5_Handler, TIM_CHANNEL_1);
}

/**
  * @brief: ???????,????,????,?????HAL_TIM_PWM_Init()??.
  * @param:  htim-?????.
  * @return: ?
  * @updata: [2017-08-07] [?????][????]
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_10;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);
}

/**
  * @description: EXTI15_10 ÖÐ¶Ï
  */
void EXTI15_10_IRQHandler(void)
{
	static uint32_t num = 0;
	static uint32_t numEnd = 0;
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
		/* USER CODE BEGIN LL_EXTI_LINE_12 */
		
		//½«ËùÓÐÍ¨µÀµÄ²ÉÑùÖµ±£´æÔÚg_SampleAdcDataÖÐ
		read_ad7616_data(g_SampleIndex);
		
	}

}

/**
  * @Description: ¸ß16Î»ÎªÍ¨µÀB£¬µÍ16Î»ÎªÍ¨µÀA
  * @param		: Í¨µÀºÅ
  * @param		: Ë÷ÒýºÅ
  * @return     : Í¨µÀAºÍÍ¨µÀBµÄ²ÉÑùÖµ
  * @updata     : [2018-11-22][Lei][´´½¨]
  */
int32_t readChanelData(uint16_t chanelNum, uint16_t adcIndex)
{
	//TODO:ÈÔ´æÔÚÒ»Ð©Ð¡ÎÊÌâ
	int16_t chanelA = 0, chanelB = 0;
	chanelA = (g_SampleAdcData[2*chanelNum][adcIndex] + g_SampleAdcData[28-2*chanelNum][adcIndex]) / 2;
	chanelB = (g_SampleAdcData[2*chanelNum+1][adcIndex] + g_SampleAdcData[29-2*chanelNum][adcIndex]) / 2;

	return (chanelA + ((int32_t)chanelB<<16));
}



