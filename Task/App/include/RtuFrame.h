/* 
 * File:   RtuFrame.h
 * Author: LiDehai
 *
 * Created on 2014年12月4日, 上午7:27
 */

#ifndef RTUFRAME_H
#define	RTUFRAME_H

#ifdef	__cplusplus
extern "C" {
#endif


#include "Header.h"


#define SEND_FRAME_LEN 128UL			//短帧发送帧长度
#define FRAME_DATA_LEN 128				//帧数据长度


typedef struct
{
    uint8_t address; //地址
    uint8_t funcode; //功能代码
    uint32_t datalen; //数据长度
    uint8_t* pData; //指向发送数据指针
    bool completeFlag;
} frameRtu;

extern uint8_t  volatile g_ReciveBufferLen;

bool FrameQueneIn(uint8_t recivData);
bool FrameQueneOut(uint8_t* pData);
void  ReciveFrameDataInit(void);
unsigned char ReciveBufferDataDealing(frameRtu* pJudgeFrame, frameRtu* pReciveFrame);
void  GenRTUFrame(uint8_t addr, uint8_t funcode,
                        uint8_t sendData[], uint32_t datalen, uint8_t* pRtuFrame, uint32_t *plen);
//void  GenRTUFrameCumulativeSum(uint8_t addr, uint8_t funcode,
//             uint8_t sendData[], uint8_t datalen, uint8_t* pRtuFrame, uint8_t *plen);
void SendFrame(uint8_t* pFrame, uint32_t len);
void  GenAndSendLongFrame(uint8_t addr, uint8_t funcode, uint8_t sendData[], uint32_t datalen);


#ifdef	__cplusplus
}
#endif

#endif	/* RTUFRAME_H */

