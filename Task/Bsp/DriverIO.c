/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DriverIO.c
  * @brief:     GPIO driver.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-08
  * @update:    [2019-01-08][Lei][creat]
  */

/*****************************include**********************************/
#include <board.h>
#include "DriverIO.h"


/***********************private variables*****************************/



/**************************function***********************************/

/**
  * @brief : 初始化GPIO
  * @param : [none].
  * @return: [RT_EOK] 
  * @updata: [2019-01-08][Lei][create]
  */  
void Init_GPIO(void)
{
  
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
  
    GPIO_Initure.Pin = GPIO_PIN_0;			//PB0
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	//PB0初始化为1
    
}
INIT_BOARD_EXPORT(Init_GPIO);

/**
  * @brief : IO口输出
  * @param : handle 该驱动的句柄
  * @param : state 输出的状态
  * @return: void
  * @updata: [2019-01-08][Lei][create]
  */
void GPIO_WriteStatus(struct IO_handle handle, int state)
{
	HAL_GPIO_WritePin(handle.GPIOx, handle.GPIOPin, (state?GPIO_PIN_SET:GPIO_PIN_RESET));
}

/**
  * @brief : IO口输出
  * @param : handle 该驱动的句柄
  * @return: 读取到的状态
  * @updata: [2019-01-08][Lei][create]
  */
int GPIO_ReadStatus(struct IO_handle handle)
{
	return HAL_GPIO_ReadPin(handle.GPIOx, handle.GPIOPin);
}



