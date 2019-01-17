/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TCP_Server.c
  * @brief:     TCP������������.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-14
  * @update:    [2019-01-14][Lei][creat]
  */

/**********************************include**************************************/
#include <rtthread.h>
#include <stdbool.h>
#include <sys/socket.h> /* ʹ��BSD Socket�ӿڱ������sockets.h���ͷ�ļ� */
#include <string.h>
#include "TCP_Server.h"
#include "RtuFrame.h"


/*******************************************************************************/
#define SERV_PORT	2345	//�˿�2345,����С��1024,1-1024Ϊϵͳ�����˿�
#define BACKLOG		5		//������еĳ���
#define BUF_SIZE	1024	//���ջ���������


static int sockfd, clientsfd;/* �������socket�����������ݴ���socket������ */


static void rt_tcp_communicate_thread_entry(void* param);
static void rt_data_deal_thread_entry(void* param);
static void ReceiveDataDeal(uint8_t* data, uint32_t dataLenth);
/**************************function***********************************/

/**
  * @brief : TCP �����ͨ�� thread
  * @param : void
  * @return: void 
  * @updata: [2019-01-14][Lei][create]
  */
void TCP_CommunicateThread(void)
{
	rt_thread_t TCP_communicate = NULL;
	TCP_communicate = rt_thread_create(TCP_COMMUNICATE_THREAD_NAME, 
									rt_tcp_communicate_thread_entry,
									RT_NULL,
									TCP_COMMUNICATE_THREAD_STACK_SIZE,
									TCP_COMMUNICATE_THREAD_PRIORITY,
									TCP_COMMUNICATE_THREAD_TIMESLICE);
	if(NULL != TCP_communicate)
	{
		rt_thread_startup(TCP_communicate);
		rt_kprintf("TCP server communicate thread start\r\n");
	}
}

/**
  * @brief : TCP ������ͨ�� thread entry
  * @param : void*
  * @return: void 
  * @updata: [2019-01-14][Lei][create]
  */
static void rt_tcp_communicate_thread_entry(void* param)
{
    char *recv_data; /* ���ڽ��յ�ָ�룬�������һ�ζ�̬��������������ڴ� */
    uint32_t sin_size;
    int	bytes_received;
    struct sockaddr_in server_addr, client_addr;/* ����IP�Ͷ˿ں���Ϣ �ͻ���IP�Ͷ˿ں���Ϣ */
    bool stop = false; /* ֹͣ��־ */

    recv_data = malloc(BUF_SIZE); /* ��������õ����ݻ��� */
    if (recv_data == NULL)
    {
        rt_kprintf("No memory\n");
        return;
    }

    /* һ��socket��ʹ��ǰ����ҪԤ�ȴ���������ָ��SOCK_STREAMΪTCP��socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* ����ʧ�ܵĴ����� */
        rt_kprintf("Socket error\n");

        /* �ͷ��ѷ���Ľ��ջ��� */
        free(recv_data);
        return;
    }

    /* ��ʼ������˵�ַ */
    server_addr.sin_family = AF_INET;   	 /* Ipv4�������� */
    server_addr.sin_port = htons(SERV_PORT); /* ����˹����Ķ˿� ������������ֽ���ת��*/
    server_addr.sin_addr.s_addr = INADDR_ANY;/* ����IP��ַ */
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));/* ��0 */

    /* ��socket������˵�ַ */
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        /* ��ʧ�� */
        rt_kprintf("Unable to bind\n");

        /* �ͷ��ѷ���Ľ��ջ��� */
        free(recv_data);
        return;
    }

    /* ��sockfd�Ͻ��м��� */
    if (listen(sockfd, BACKLOG) == -1)
    {
        rt_kprintf("Listen error\n");

        /* �ͷ��ѷ���Ľ��ջ��� */
        free(recv_data);
        return;
    }

    rt_sprintf(recv_data, "%4d", SERV_PORT);
    rt_kprintf("\nTCPServer Waiting for client on port %s...\n",recv_data);

    while (stop != true)
    {
        sin_size = sizeof(struct sockaddr_in);

        /* ����һ���ͻ�������socket�����������������������ʽ�� */
        clientsfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        /* ���ص������ӳɹ���socket��������������ʹ�����������������ͨ�� */

        /* ��ӡ�ͻ��˵�IP�Ͷ˿ں� */
        rt_kprintf("I got a connection from (IP: %s ,PORT: %d)\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* �ͻ������ӵĴ��� */
        while (1)
        {

            /* ��connected socket�н������ݣ�����buffer��1024��С��������һ���ܹ��յ�1024��С������ */
            bytes_received = recv(clientsfd, recv_data, BUF_SIZE, 0);
            if (bytes_received <= 0)
            {
                /* ����ʧ�ܣ��ر����connected socket */
                closesocket(clientsfd);
                break;
            }

			ReceiveDataDeal((uint8_t*)recv_data, bytes_received);
            
			rt_thread_delay(50);
        }
    }

    /* �ر�socket �˳����� */
    closesocket(sockfd);

    /* �ͷŽ��ջ��� */
    free(recv_data);

    return ;
}


/**
  * @brief : TCP ������ͨ�ŷ��ͺ���
  * @param : data Ҫ���͵�����
  * @param : dataLenth ���ݳ���
  * @return: void 
  * @updata: [2019-01-14][Lei][create]
  */
void TCP_SendData(uint8_t* data, uint32_t dataLenth)
{
	send(clientsfd, data, dataLenth, 0);
}

/**
  * @brief : TCP ������ͨ�ŷ��ͺ���
  * @param : addr Զ���ĵ�ַ��
  * @param : funcode ������
  * @param : data Ҫ���͵�����
  * @return: void 
  * @updata: [2019-01-16][Lei][create]
  */
void PackAndSendData(uint8_t addr, uint8_t funcode, uint8_t* data)
{
	uint8_t sendData[SEND_FRAME_LEN] = {0};		//Ҫ���͵����ݰ�
	uint32_t dataLenth = 0;						//Ҫ���͵����ݰ���С
	
	GenRTUFrame(addr, funcode, data, strlen((char*)data), sendData, &dataLenth);
	SendFrame(sendData, dataLenth);
}


/**
  * @brief : TCP ������ͨ�ŶԽ��յ������ݽ�����Ӵ���
  * @param : data Ҫ���͵�����
  * @param : dataLenth ���ݳ���
  * @return: void 
  * @updata: [2019-01-14][Lei][create]
  */
static void ReceiveDataDeal(uint8_t* data, uint32_t dataLenth)
{
	for(uint32_t i = 0; i < dataLenth; i++)
	{
		FrameQueneIn((uint8_t)data[i]);
	}
}


/**
  * @brief : ���ݴ����߳�
  * @param : void
  * @return: void 
  * @updata: [2019-01-16][Lei][create]
  */
void DataDealThread(void)
{
	rt_thread_t DataDeal = NULL;
	DataDeal = rt_thread_create(DATA_DEAL_THREAD_NAME, 
								rt_data_deal_thread_entry,
								RT_NULL,
								DATA_DEAL_THREAD_STACK_SIZE,
								DATA_DEAL_THREAD_PRIORITY,
								DATA_DEAL_THREAD_TIMESLICE);
	if(NULL != DataDeal)
	{
		rt_thread_startup(DataDeal);
		rt_kprintf("Data deal thread start\r\n");
	}
}


/**
  * @brief : ���ݴ����߳����
  * @param : void*
  * @return: void 
  * @updata: [2019-01-16][Lei][create]
  */
static void rt_data_deal_thread_entry(void* param)
{
	uint32_t count = 0;						//��ǰ���ն�����ʣ���ַ���
	uint32_t lastCount = 0;					//��һ�ν��ն�����ʣ����ַ���
	uint8_t realData[100] = {0};			//�ӽ��յ���֡��Ϣ����ȡ����������Ϣ
	frameRtu judgeFrame, reciveFrame;		//����֡�������Ϣ�ṹ��
	judgeFrame.address = LOCAL_ADDRESS;				//���ñ��ص�ַ��Ϣ
	judgeFrame.funcode = FUN_CODE;				//���ù����룬��Ӧֻ���շֺ�բ��Ϣ
	
	ReciveFrameDataInit();
	while(1)
	{
		if(g_ReciveBufferLen)
		{
			do
			{	
				count = ReciveBufferDataDealing(&judgeFrame, &reciveFrame);
				if (reciveFrame.completeFlag)		//������һ֡�����������˳���ѭ�����д���
				{
					memset(realData, 0, sizeof(realData));
					memcpy(realData, reciveFrame.pData, strlen((char*)reciveFrame.pData));
					memset(&reciveFrame, 0, sizeof(frameRtu));
					break;
				}
				
				if (lastCount == count)			//��һ�ε�ʣ���ַ����뱾��ʣ���ַ���������ͬ���˳���ѭ��
				{
					break;
				}
				lastCount = count;		//��¼����ʣ�����ݳ���
				
				rt_thread_delay(30);
			}while(true);
				
		}
		if(strlen((char*)realData))
		{
			rt_kprintf("REAL_DATA = %s\r\n", realData);
			memset(realData, 0, strlen((char*)realData));
		}
		
		rt_thread_delay(30);
	}
	
}

