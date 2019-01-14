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

/*******************************************************************************/
#define SERV_PORT	2345	//端口2345,不能小于1024,1-1024为系统保留端口
#define BACKLOG		5		//请求队列的长度
#define BUF_SIZE	1024	//接收缓冲区长度

static const char send_data[] = "This is TCP Server from RT-Thread.\r\n"; /* 发送用到的数据 */


static void rt_tcp_communicate_thread_entry(void* param);
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
    int sockfd, clientsfd;/* 定义监听socket描述符和数据传输socket描述符 */
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
            /* 发送数据到connected socket */
            send(clientsfd, send_data, strlen(send_data), 0);

            /* 从connected socket中接收数据，接收buffer是1024大小，但并不一定能够收到1024大小的数据 */
            bytes_received = recv(clientsfd, recv_data, BUF_SIZE, 0);
            if (bytes_received <= 0)
            {
                /* 接收失败，关闭这个connected socket */
                closesocket(clientsfd);
                break;
            }

            /* 有接收到数据，把末端清零即加入字符串结束符*/
            recv_data[bytes_received] = '\0';
            if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
            {
                /* 如果首字母是q或Q，关闭这个连接 */
                closesocket(clientsfd);
                break;
            }
            else if (strcmp(recv_data, "exit") == 0)
            {
                /* 如果接收的是exit，则关闭整个服务端 */
                closesocket(clientsfd);
                stop = true;
                break;
            }
            else
            {
                /* 在控制终端显示收到的数据 */
                rt_kprintf("RECIEVED DATA = %s \n" , recv_data);
            }
			rt_thread_delay(50);
        }
    }

    /* 关闭socket 退出服务 */
    closesocket(sockfd);

    /* 释放接收缓冲 */
    free(recv_data);

    return ;
}



