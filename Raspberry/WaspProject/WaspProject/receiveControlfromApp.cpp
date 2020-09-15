#include <iostream>
using namespace std;
#include <string.h>
#include <stdlib.h>
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "detectState.h"
#include <arpa/inet.h>
#include <wiringPi.h>
#include <unistd.h>
#include "Threadfunction.h"
#include "socketParameter.h"

bool isSocketConnect = false;
int sock;
void* receiveControl_thread(void*)
{
	printf("Execute receive Control\n");
	char control_data[4]= {0,};
	ssize_t num;
	pthread_t timer_thread;

	struct sockaddr_in serv_addr;
	
	if(sock = socket(AF_INET,SOCK_STREAM,0)==-1)
	{
		printf("Error create socket\n");
		exit(1);
	}
	sock = socket(AF_INET,SOCK_STREAM,0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(DESTINATION_IP);
	serv_addr.sin_port = htons(PORT);
	
	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1)
	{
		printf("Error connect to server\n");
		close(sock);
		exit(1);
	}
	isSocketConnect = true;
	printf("Connect to Server\n");
	while(1)
	{
		num = recv(sock,control_data,sizeof(control_data),0);
		if(num==-1)
		{
			printf("receive data is fail.\n");
			exit(1);
		}
		printf("Receive data : %d\n",*((int*)control_data));
		if(*((int*)control_data)==0)
		{
				ifwaspdetected(timer_thread);
		}
	}
	isSocketConnect = false;
	close(sock);
}
