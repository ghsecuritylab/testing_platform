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


#define LOCAL_ADDRESS	0XDF		//���صĵ�ַ
#define REMOTE_ADDRESS	0xFD		//Զ���ĵ�ַ


//������
#define FUN_CODE  	0XED		//�·�����
#define UP_CODE 	0XDE		//�ϴ�����



#define Reset() {__asm__ volatile ("RESET");}
#endif
