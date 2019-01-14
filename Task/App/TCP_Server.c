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

/*******************************************************************************/
#define SERV_PORT	2345	//�˿�2345,����С��1024,1-1024Ϊϵͳ�����˿�
#define BACKLOG		5		//������еĳ���
#define BUF_SIZE	1024	//���ջ���������

static const char send_data[] = "This is TCP Server from RT-Thread.\r\n"; /* �����õ������� */


static void rt_tcp_communicate_thread_entry(void* param);
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
    int sockfd, clientsfd;/* �������socket�����������ݴ���socket������ */
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
            /* �������ݵ�connected socket */
            send(clientsfd, send_data, strlen(send_data), 0);

            /* ��connected socket�н������ݣ�����buffer��1024��С��������һ���ܹ��յ�1024��С������ */
            bytes_received = recv(clientsfd, recv_data, BUF_SIZE, 0);
            if (bytes_received <= 0)
            {
                /* ����ʧ�ܣ��ر����connected socket */
                closesocket(clientsfd);
                break;
            }

            /* �н��յ����ݣ���ĩ�����㼴�����ַ���������*/
            recv_data[bytes_received] = '\0';
            if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
            {
                /* �������ĸ��q��Q���ر�������� */
                closesocket(clientsfd);
                break;
            }
            else if (strcmp(recv_data, "exit") == 0)
            {
                /* ������յ���exit����ر���������� */
                closesocket(clientsfd);
                stop = true;
                break;
            }
            else
            {
                /* �ڿ����ն���ʾ�յ������� */
                rt_kprintf("RECIEVED DATA = %s \n" , recv_data);
            }
			rt_thread_delay(50);
        }
    }

    /* �ر�socket �˳����� */
    closesocket(sockfd);

    /* �ͷŽ��ջ��� */
    free(recv_data);

    return ;
}



