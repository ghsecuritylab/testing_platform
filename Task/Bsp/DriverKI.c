/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      DriverIO.c
  * @brief:     GPIO driver.
  * @version:   V1.0.0 
  * @author:    ZhaoChangquan
  * @date:      2019-01-09
  * @update:    [2019-01-09][ZhaoChangquan][creat]
  */

/*****************************include**********************************/
#include <board.h>
#include "DriverIO.h"
#include "DriverKI.h"




static uint16_t KIBuffer;   //缓存16路开入输入   


  struct IO_handle KICS1Handle;
  struct IO_handle KICS2Handle;
	struct IO_handle KI1Handle;
	struct IO_handle KI2Handle;
	struct IO_handle KI3Handle;
	struct IO_handle KI4Handle;
	struct IO_handle KI5Handle;
	struct IO_handle KI6Handle;
	struct IO_handle KI7Handle;
	struct IO_handle KI8Handle;
	
	
	//分合闸继电器控制信号
	struct IO_handle KO1Handle;
	struct IO_handle KO2Handle;

 


/**************************function***********************************/

/**
  * @brief : KI
  * @param : [none].
  * @return: Buffer 
  * @updata: [2019-01-09][ZhaoChangquan][add]
  */  
void KIInit(void)
{
	
	KICS1Handle.GPIOPin = GPIO_PIN_1;
	KICS1Handle.GPIOx = GPIOB;
	KICS1Handle.Write = GPIO_WriteStatus;    
	KICS1Handle.Read = NULL;
	
	KICS2Handle.GPIOPin = GPIO_PIN_12;
	KICS2Handle.GPIOx = GPIOG;
	KICS2Handle.Write = GPIO_WriteStatus;
	KICS2Handle.Read = NULL;
	
	KI1Handle.GPIOPin = GPIO_PIN_3;
	KI1Handle.GPIOx = GPIOD;
	KI1Handle.Write = NULL;
	KI1Handle.Read = GPIO_ReadStatus;
	
	KI2Handle.GPIOPin = GPIO_PIN_5;
	KI2Handle.GPIOx = GPIOH;
	KI2Handle.Write = NULL;
	KI2Handle.Read = GPIO_ReadStatus;
	
	KI3Handle.GPIOPin = GPIO_PIN_3;
	KI3Handle.GPIOx = GPIOA;
	KI3Handle.Write = NULL;
	KI3Handle.Read = GPIO_ReadStatus;
	
	KI4Handle.GPIOPin = GPIO_PIN_7;
	KI4Handle.GPIOx = GPIOD;
	KI4Handle.Write = NULL;
	KI4Handle.Read = GPIO_ReadStatus;
	
	KI5Handle.GPIOPin = GPIO_PIN_4;
	KI5Handle.GPIOx = GPIOA;
	KI5Handle.Write = NULL;
	KI5Handle.Read = GPIO_ReadStatus;
	
	KI6Handle.GPIOPin = GPIO_PIN_5;
	KI6Handle.GPIOx = GPIOA;
	KI6Handle.Write = NULL;
	KI6Handle.Read = GPIO_ReadStatus;
	
	KI7Handle.GPIOPin = GPIO_PIN_6;
	KI7Handle.GPIOx = GPIOD;
	KI7Handle.Write = NULL;
	KI7Handle.Read = GPIO_ReadStatus;
	
	KI8Handle.GPIOPin = GPIO_PIN_6;
	KI8Handle.GPIOx = GPIOA;
	KI8Handle.Write = NULL;
	KI8Handle.Read = GPIO_ReadStatus;
	
	
}



/**
  * @brief : KI_Read
  * @param : Read KI
  * @return: void
  * @updata: [2019-01-10][ZhaoChangquan][creat]	           
  */  

uint8_t KI_Read(struct IO_handle KIHandle)
{
	uint8_t ki;
	ki=KIHandle.Read(KIHandle);
	return ki;
}




/**
  * @brief : KI_CS_Write
  * @param : Write HIGH|LOW to KI_CS
  * @return: void
  * @updata: [2019-01-10][ZhaoChangquan][creat]	           
  */  
void KI_CS_Write(struct IO_handle KICSHandle, int state)
{
	
	KICSHandle.Write(KICSHandle,state);
	
}




/**
  * @brief : KI_Buffer
  * @param : Read KIBits and save as KIByte
  * @return: 返回开入量字节
  * @updata: [2019-01-10][ZhaoChangquan][creat]
	           
  */  
uint8_t KI_Buffer(void)
{
	uint8_t KIBufferValue;
	KIBufferValue=((KI_Read(KI8Handle)<<7)|(KI_Read(KI7Handle)<<6)|(KI_Read(KI6Handle)<<5)|(KI_Read(KI5Handle)<<4)|(KI_Read(KI4Handle)<<3)|(KI_Read(KI3Handle)<<2)|(KI_Read(KI2Handle)<<1)|KI_Read(KI1Handle));
	return KIBufferValue;
}



/**
  * @brief : KI_Get
  * @param : Save KIByte
  * @return: void
  * @updata: [2019-01-09][ZhaoChangquan][creat]
	           
  */  
uint16_t KI_Get(void)
{
	
	KI_CS_Write(KICS2Handle, LOW);
	KI_CS_Write(KICS2Handle, LOW);
	KI_CS_Write(KICS2Handle, LOW);
	KI_CS_Write(KICS2Handle, LOW);
	KIBuffer = KI_Buffer();
	KI_CS_Write(KICS2Handle, HIGH);
	
	KI_CS_Write(KICS1Handle, LOW);
	KI_CS_Write(KICS1Handle, LOW);
	KI_CS_Write(KICS1Handle, LOW);
	KI_CS_Write(KICS1Handle, LOW);
	KIBuffer = (KI_Buffer())|(KIBuffer<<8);
	KI_CS_Write(KICS1Handle, HIGH);
	

	
	
	return KIBuffer;
}







