/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TCP_Server.h
  * @brief:     TCP������������.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-14
  * @update:    [2019-01-14][Lei][creat]
  */


#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <stdint.h>


#define TCP_COMMUNICATE_THREAD_NAME                                                "tcp"
#define TCP_COMMUNICATE_THREAD_PRIORITY                                            (6)
#define TCP_COMMUNICATE_THREAD_STACK_SIZE                                          (4096)
#define TCP_COMMUNICATE_THREAD_TIMESLICE                                           (20)

#define DATA_DEAL_THREAD_NAME                                                		"data_deal"
#define DATA_DEAL_THREAD_PRIORITY                                            		(6)
#define DATA_DEAL_THREAD_STACK_SIZE                                          		(2048)
#define DATA_DEAL_THREAD_TIMESLICE                                           		(20)



void TCP_CommunicateThread(void);					//TCPͨ���߳�
void TCP_SendData(uint8_t* data, uint32_t dataLenth);		//TCP��������
void DataDealThread(void);						//���ݴ����߳�
void PackAndSendData(uint8_t addr, uint8_t funcode, uint8_t* data);		//�����������



#endif

