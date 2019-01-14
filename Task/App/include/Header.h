#ifndef _Header_H_
#define _Header_H_

#define FCY 16000000

#include <stdint.h>
#include <stdbool.h>

typedef struct TagPointUint8
{
    uint8_t* pData;
    uint8_t len;
}PointUint8;


#define LOCAL_ADDRESS 0xA1
#define MAIN_ADDRESS 0x1A

#define LOCAL_MAC_ID 0xF1

//功能码
#define FUN_CODE  0x55		//下发命令
#define UP_CODE 0xAA		//上传数据


#define  UART1_MODE 0xAA
#define  UART2_MODE 0x55
#define  U485_MODE 0xA5

#define WORK_MODE  UART1_MODE

#define Reset() {__asm__ volatile ("RESET");}
#endif
