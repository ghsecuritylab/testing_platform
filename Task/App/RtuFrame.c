#include "RtuFrame.h"
//#include "CRC16.h"
#include <string.h>
#include "TCP_Server.h"
#include "rtthread.h"

/******************************************
//针对此种情况出队与入队可能发生冲突。
//改在出队时，禁止串口接收中断。 2015/10/15
//增大缓冲帧容量由16改为64
//每次进入帧处理队列后刷新接收使能位
//增加  ReciveErrorFlag 接收错误标志 及接收满后的重新清空初始化
********************************************/

//#include <stdlib.h>
//入队错误标志位 true未满 正常入队 false 无法入队
bool volatile ReciveErrorFlag = true; 

#define FRAME_QUENE_LEN 512
uint8_t  volatile g_ReciveBufferLen = 0; //未处理接收数据长度
uint8_t  volatile  g_ReciveBuffer[FRAME_QUENE_LEN] = {0}; //临时存放串口接收数据
uint8_t  FifoHead = 0;
uint8_t  FifoEnd =0;
uint8_t  DealStep = 0;

//帧数据放置区域

uint8_t  volatile FrameData[FRAME_DATA_LEN] = {0};

uint8_t RtuFrame[16] = {0};
uint8_t completeFlag = 0;

uint8_t LocalAddress =  LOCAL_ADDRESS;

uint32_t ReciveIndex = 0;  //接收索引 指向待存帧位置

uint8_t  volatile SendFrameData[SEND_FRAME_LEN] = {0};

static uint16_t CheckSum(uint8_t *dataMsg, uint32_t dataLen);
/**************************************************
 *函数名： ReciveFrameDataInit()
 *功能：  初始化此文件模块有关变量
 *形参： void
 *返回值：void
****************************************************/
void  ReciveFrameDataInit(void)
{
    g_ReciveBufferLen = 0;
    FifoHead = 0;
    FifoEnd = 0;
    
    ReciveErrorFlag = true;
    LocalAddress =  LOCAL_ADDRESS;
    DealStep = 0;
    ReciveIndex = 0;
    completeFlag = 0;
}

/**************************************************
 *函数名： FrameQueneIn()
 *功能：  帧队列入队
 *形参：入队数据 uint8_t * pData
 *返回值：如果队列为满，则返回false，否则为TURE
****************************************************/
bool FrameQueneIn(uint8_t recivData)
{
    //队列未满
    if (g_ReciveBufferLen < FRAME_QUENE_LEN)
    {
        g_ReciveBufferLen++;
        g_ReciveBuffer[FifoEnd] = recivData; //入队
        FifoEnd =( FifoEnd + 1)% FRAME_QUENE_LEN;

        return true;
    }
    return false;
}
/**************************************************
 * 函数名：FrameQueneOut()
 * 功能：   帧队列出队
 * 形参：出队数据 uint8_t* pData
 * 返回值：如果为队列空则返回false，否则为TURE
****************************************************/
bool FrameQueneOut(uint8_t* pData)
{
    //队列是否为空

    if (g_ReciveBufferLen > 0)
    {
        g_ReciveBufferLen--;
        *pData = g_ReciveBuffer[FifoHead]; //首先出队
        FifoHead =( FifoHead + 1)% FRAME_QUENE_LEN;

        return true;
    }

    return false;
}


/*************************************************
 *函数名：	ReciveBufferDataDealing()
 *功能：	对接收数据进行处理
 *形参：
 *返回值：
****************************************************/
uint8_t ReciveBufferDataDealing(frameRtu* pJudgeFrame, frameRtu* pReciveFrame)
{

    //如果存在未处理的数据
    if (g_ReciveBufferLen > 0)
    {
        switch(DealStep)
        {
            //判断地址
            case 0:
            {
                uint8_t data = 0;
                pReciveFrame->completeFlag = false;
                FrameQueneOut(&data);

                if (pJudgeFrame->address == data) //地址符合则进行下一轮判断
                {
                    pReciveFrame->address =  data;
                    DealStep  = 1;
                }
				else
				{
					DealStep  = 0;
				}
                break;
            }
            //获取功能代码
            case 1:
            {
                uint8_t data = 0;
                FrameQueneOut(&data);
				if (pJudgeFrame->funcode == data) //功能码符合则进行下一轮判断
                {
                    pReciveFrame->funcode = data;
					DealStep = 2;
                }
                else
				{
					DealStep = 0;
				}
                break;
            }
            //获取数据字节长度
            case 2:
            {
                uint32_t dataLenth = 0;
				uint8_t data = 0;
				for(uint8_t n = 0; n < 4; n++)
				{
					FrameQueneOut(&data);
					dataLenth = dataLenth | ((uint32_t)data << (8*n));
				}
                
                pReciveFrame->datalen = dataLenth;
                DealStep = 3;
                ReciveIndex = 6;
                break;
            }
            //进行校验
            case 3:
            {
                //应加计时防止此处长时间接收不到
                //接收缓冲数据应大于等于数据长度
                //转存数据
                FrameQueneOut((uint8_t*)&FrameData[ReciveIndex++]);// ReciveIndex++; //接收数据则索引加一

                if (ReciveIndex > FRAME_DATA_LEN)
                {
                    ReciveIndex = 6;
                    DealStep = 0;
                    break;
                    //接收帧长度过长丢弃
                }
                uint32_t len = pReciveFrame->datalen;
                if (ReciveIndex >= len + 8)
                {
                    FrameData[0] = pReciveFrame->address;
                    FrameData[1] = pReciveFrame->funcode;
					FrameData[2] = (len & 0x000000FF) >> 0;			//长度的低位
					FrameData[3] = (len & 0x0000FF00) >> 8;
					FrameData[4] = (len & 0x000FF000) >> 16;
                    FrameData[5] = (len & 0xFF000000) >> 24;		//长度的高位
                    uint16_t crc =  CheckSum((uint8_t* )FrameData, len + 6);
                    uint8_t  crcL = FrameData[len + 6];
                    uint8_t  crcH = FrameData[len + 7];
                    //若校验吻合,则执行下一步动作
                    if (crc == ((uint16_t)crcH<<8  | crcL))
                    {
                        pReciveFrame->pData = (uint8_t*)(&FrameData[6]);
						pReciveFrame->pData[len] = '\0';
                        pReciveFrame->completeFlag = true;
                    }
            
                    DealStep = 0;
                }
                break;
            }
            default:
            {
                DealStep = 0;
                ReciveIndex = 6;
                break;
            }
        }
    }
    return g_ReciveBufferLen; //返回剩余长度
}

/**************************************************
 *函数名：  GenRTUFrame()
 *功能：  生成通信帧
 *形参： 目的地址 uint8_t addr
 *       功能代号 uint8_t funcode
 *       数据数组  uint8_t sendData[]
 *       数据长度  uint32_t datalen
 *       生成的帧指针 uint8_t* pRtuFrame
 *       帧总长度 uint32_t *plen
 *返回值：void
****************************************************/
void  GenRTUFrame(uint8_t addr, uint8_t funcode,
             uint8_t sendData[], uint32_t datalen, uint8_t* pRtuFrame, uint32_t *plen)
{
    uint32_t len = 1 + 1 + 4 + datalen + 2;
    *plen = len;

    pRtuFrame[0] = addr;
    pRtuFrame[1] = funcode;
    pRtuFrame[2] = (datalen & 0x000000FF) >> 0;
	pRtuFrame[3] = (datalen & 0x0000FF00) >> 8;
	pRtuFrame[4] = (datalen & 0x000FF000) >> 16;
	pRtuFrame[5] = (datalen & 0x0FF00000) >> 24;
	

    int i = 0;
    for ( i = 0; i < datalen;  i++)
    {
        pRtuFrame[i + 6] = sendData[i];
    }

//    uint16_t crc =  CRC16(pRtuFrame, len - 2);
	uint16_t crc =  CheckSum(pRtuFrame, len - 2);
    pRtuFrame[len - 2] = (uint8_t)(crc & 0xFF);
    pRtuFrame[len - 1] = (uint8_t)((crc & 0xFF00) >> 8);

    completeFlag = 0;
}



void SendFrame(uint8_t* pFrame, uint32_t len)
{
	TCP_SendData(pFrame, len);
}


/**
  * @brief : 累加和校验
  * @param : puchMsg
  * @param : usDataLen
  * @return: 累加和结果 
  * @updata: [2019-01-16][Lei][create]
  */
static uint16_t CheckSum(uint8_t *dataMsg, uint32_t dataLen)
{
	uint16_t sum = 0;
	for(uint32_t i = 0; i < dataLen; i++)
	{
		sum += dataMsg[i];
	}
	return sum;
}

