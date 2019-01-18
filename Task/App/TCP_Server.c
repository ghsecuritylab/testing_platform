/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      TCP_Server.c
  * @brief:     TCP服务端相关任务.
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-01-14
  * @update:    [2019-01-14][Lei][creat]
  */

/**********************************include**************************************/
#include <rtthread.h>
#include <stdbool.h>
#include <sys/socket.h> /* 使用BSD Socket接口必须包含sockets.h这个头文件 */
#include <string.h>
#include "TCP_Server.h"
#include "RtuFrame.h"


/*******************************************************************************/
#define SERV_PORT	2345	//端口2345,不能小于1024,1-1024为系统保留端口
#define BACKLOG		5		//请求队列的长度
#define BUF_SIZE	1024	//接收缓冲区长度


static int sockfd, clientsfd;/* 定义监听socket描述符和数据传输socket描述符 */


static void rt_tcp_communicate_thread_entry(void* param);
static void rt_data_deal_thread_entry(void* param);
static void ReceiveDataDeal(uint8_t* data, uint32_t dataLenth);
/**************************function***********************************/

/**
  * @brief : TCP 服务端通信 thread
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
  * @brief : TCP 服务器通信 thread entry
  * @param : void*
  * @return: void 
  * @updata: [2019-01-14][Lei][create]
  */
static void rt_tcp_communicate_thread_entry(void* param)
{
    char *recv_data; /* 用于接收的指针，后面会做一次动态分配以请求可用内存 */
    uint32_t sin_size;
    int	bytes_received;
    struct sockaddr_in server_addr, client_addr;/* 本机IP和端口号信息 客户端IP和端口号信息 */
    bool stop = false; /* 停止标志 */

    recv_data = malloc(BUF_SIZE); /* 分配接收用的数据缓冲 */
    if (recv_data == NULL)
    {
        rt_kprintf("No memory\n");
        return;
    }

    /* 一个socket在使用前，需要预先创建出来，指定SOCK_STREAM为TCP的socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /* 创建失败的错误处理 */
        rt_kprintf("Socket error\n");

        /* 释放已分配的接收缓冲 */
        free(recv_data);
        return;
    }

    /* 初始化服务端地址 */
    server_addr.sin_family = AF_INET;   	 /* Ipv4因特网域 */
    server_addr.sin_port = htons(SERV_PORT); /* 服务端工作的端口 这里进行网络字节序转换*/
    server_addr.sin_addr.s_addr = INADDR_ANY;/* 本机IP地址 */
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));/* 清0 */

    /* 绑定socket到服务端地址 */
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        /* 绑定失败 */
        rt_kprintf("Unable to bind\n");

        /* 释放已分配的接收缓冲 */
        free(recv_data);
        return;
    }

    /* 在sockfd上进行监听 */
    if (listen(sockfd, BACKLOG) == -1)
    {
        rt_kprintf("Listen error\n");

        /* 释放已分配的接收缓冲 */
        free(recv_data);
        return;
    }

    rt_sprintf(recv_data, "%4d", SERV_PORT);
    rt_kprintf("\nTCPServer Waiting for client on port %s...\n",recv_data);

    while (stop != true)
    {
        sin_size = sizeof(struct sockaddr_in);

        /* 接受一个客户端连接socket的请求，这个函数调用是阻塞式的 */
        clientsfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        /* 返回的是连接成功的socket描述符，后续将使用这个描述符来进行通信 */

        /* 打印客户端的IP和端口号 */
        rt_kprintf("I got a connection from (IP: %s ,PORT: %d)\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* 客户端连接的处理 */
        while (1)
        {

            /* 从connected socket中接收数据，接收buffer是1024大小，但并不一定能够收到1024大小的数据 */
            bytes_received = recv(clientsfd, recv_data, BUF_SIZE, 0);
            if (bytes_received <= 0)
            {
                /* 接收失败，关闭这个connected socket */
                closesocket(clientsfd);
                break;
            }

			ReceiveDataDeal((uint8_t*)recv_data, bytes_received);
            
			rt_thread_delay(50);
        }
    }

    /* 关闭socket 退出服务 */
    closesocket(sockfd);

    /* 释放接收缓冲 */
    free(recv_data);

    return ;
}


/**
  * @brief : TCP 服务器通信发送函数
  * @param : data 要发送的数据
  * @param : dataLenth 数据长度
  * @return: void 
  * @updata: [2019-01-14][Lei][create]
  */
void TCP_SendData(uint8_t* data, uint32_t dataLenth)
{
	send(clientsfd, data, dataLenth, 0);
}

/**
  * @brief : TCP 服务器通信发送函数
  * @param : addr 远方的地址码
  * @param : funcode 功能码
  * @param : data 要发送的数据
  * @return: void 
  * @updata: [2019-01-16][Lei][create]
  */
void PackAndSendData(uint8_t addr, uint8_t funcode, uint8_t* data)
{
	uint8_t sendData[SEND_FRAME_LEN] = {0};		//要发送的数据包
	uint32_t dataLenth = 0;						//要发送的数据包大小
	
	GenRTUFrame(addr, funcode, data, strlen((char*)data), sendData, &dataLenth);
	SendFrame(sendData, dataLenth);
}


/**
  * @brief : TCP 服务器通信对接收到的数据进行入队处理
  * @param : data 要发送的数据
  * @param : dataLenth 数据长度
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
  * @brief : 数据处理线程
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
  * @brief : 数据处理线程入口
  * @param : void*
  * @return: void 
  * @updata: [2019-01-16][Lei][create]
  */
static void rt_data_deal_thread_entry(void* param)
{
	uint32_t count = 0;						//当前接收队列中剩余字符数
	uint32_t lastCount = 0;					//上一次接收队列中剩余的字符数
	uint8_t realData[100] = {0};			//从接收到的帧信息中提取出的数据信息
	frameRtu judgeFrame, reciveFrame;		//接收帧的相关信息结构体
	judgeFrame.address = LOCAL_ADDRESS;				//设置本地地址信息
	judgeFrame.funcode = FUN_CODE;				//设置功能码，对应只接收分合闸信息
	
	ReciveFrameDataInit();
	while(1)
	{
		if(g_ReciveBufferLen)
		{
			do
			{	
				count = ReciveBufferDataDealing(&judgeFrame, &reciveFrame);
				if (reciveFrame.completeFlag)		//接受完一帧完整数据则退出死循环进行处理
				{
					memset(realData, 0, sizeof(realData));
					memcpy(realData, reciveFrame.pData, strlen((char*)reciveFrame.pData));
					memset(&reciveFrame, 0, sizeof(frameRtu));
					break;
				}
				
				if (lastCount == count)			//上一次的剩余字符数与本次剩余字符数数量相同则退出死循环
				{
					break;
				}
				lastCount = count;		//记录本次剩余数据长度
				
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

