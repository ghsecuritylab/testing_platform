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
	           [2019-01-09][ZhaoChangquan][add]
  */  
void Init_GPIO(void)
{
  
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
	  __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOD时钟
		__HAL_RCC_GPIOH_CLK_ENABLE();           //开启GPIOH时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
		__HAL_RCC_GPIOG_CLK_ENABLE();           //开启GPIOG时钟
	  __HAL_RCC_GPIOI_CLK_ENABLE();           //开启GPIOI时钟
  
	//PB1--KI_CS1   PG12--KI_CS2     初始化为1，0有效
    GPIO_Initure.Pin = GPIO_PIN_0|GPIO_PIN_1;			//PB0  PB1
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
	
	  GPIO_Initure.Pin = GPIO_PIN_12;      //PG12  高速 上拉 推挽输出
	  HAL_GPIO_Init(GPIOG,&GPIO_Initure);
	
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_SET);   //PB0|PB1初始化为1
	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);      //PG12初始化为1

  //PD3--KI1   PH5--KI2   PA3--KI3   PD7--KI4   PA4--KI5   PA5--KI6   PD6--KI7   PA6--KI8
		GPIO_Initure.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;   //PA3  PA4  PA5 PA6
		GPIO_Initure.Mode = GPIO_MODE_INPUT;   //输入
		GPIO_Initure.Pull = GPIO_PULLUP;    //上拉
		GPIO_Initure.Speed = GPIO_SPEED_HIGH;   //高速
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		GPIO_Initure.Pin = GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7;    //PD3  PD6   PD7
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);
		
		GPIO_Initure.Pin = GPIO_PIN_5;     //PH5
		HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
		
		//分合闸信号端口初始化   PI6--KIKO10--分闸   PG7--KIKO12--合闸
		GPIO_Initure.Pin = GPIO_PIN_7;			//PG7
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);
		
		GPIO_Initure.Pin = GPIO_PIN_6;			//PI6
		HAL_GPIO_Init(GPIOI, &GPIO_Initure);
		
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);   //PG7初始化为1
	  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, GPIO_PIN_SET);   //PI6初始化为1
		
		
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



