#include <iostream>
using namespace std;
#include <string.h>
#include <stdlib.h>
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <unistd.h>
#include <arpa/inet.h>
#include "socketParameter.h"
#include "video.h"
pthread_mutex_t forqueue;
using namespace cv;
bool isconnectedtoserver = false;
void * send_VideoThread(void* )
{
	int sock_video = 0;
	struct sockaddr_in serv_addr_video;

	if(sock_video = socket(AF_INET,SOCK_STREAM,0)==-1)
	{
		printf("Error create socket\n");
		exit(1);
	}
	sock_video = socket(AF_INET,SOCK_STREAM,0);
	serv_addr_video.sin_family = AF_INET;
	serv_addr_video.sin_addr.s_addr = inet_addr(DESTINATION_IP);
	serv_addr_video.sin_port = htons(PORT_VIDEO);
	
	if(connect(sock_video, (struct sockaddr *)&serv_addr_video, sizeof(serv_addr_video))==-1)
	{
		printf("Error connect to server\n");
		close(sock_video);
		exit(1);
	}
	isconnectedtoserver = true;
	delay(50);
	printf("Connect to video Socekt Server\n");
	
	/*for(int i = 0; i<3;i++)
	{
		for(int j = 0; j<20;j++)
		{
			send(sock_video,frame[i][j],sizeof(frame[i][j]),0);	
		}
	}
	*/
	while(1)
	{
		pthread_mutex_lock(&forqueue);
		if(videoQueue.empty()==false)
		{
			Mat trans_image;
			int row_image = 100;
			int col_image = 170;
			char f[1020] = {0};
			cv::resize(videoQueue.front(),trans_image,cv::Size(col_image,row_image));
			videoQueue.pop();
			printf("send\n");
			int col = 0;
			int row = 0;
			int j = 0;
			//int count = 0;
			for(row = 0; row<row_image;row++)
			{
				for(col = 0 ; col<col_image ;col++)
				{
					//printf("execute here %d\n",col);
					f[j] = trans_image.at<Vec3b>(row,col)[0]; //B
					j++;
					if(j==1020)
					{
							//pthread_mutex_lock(&forqueue);
						send(sock_video,(char*)&f,1020,0);
						//delay(5);
							//videoQueue.push(f);
							//pthread_mutex_unlock(&forqueue);
							//printf("insert to queue : %d\n",count);
						j=0;
							//count++;
					}
					f[j] = trans_image.at<Vec3b>(row,col)[1]; //G
					j++;
					if(j==1020)
					{
							//pthread_mutex_lock(&forqueue);
						send(sock_video,(char*)&f,1020,0);
						//delay(5);
							//videoQueue.push(f);
							//pthread_mutex_unlock(&forqueue);
							//printf("insert to queue : %d\n",count);
						j=0;
						//count++;
					}
					f[j] = trans_image.at<Vec3b>(row,col)[2]; //R
					j++;
					if(j==1020)
					{
						send(sock_video,(char*)&f,1020,0);
						//delay(5);
							//pthread_mutex_lock(&forqueue);
							//videoQueue.push(f);
							//pthread_mutex_unlock(&forqueue);
							//printf("insert to queue : %d\n",count);
						j=0;
						//count++;
					}
				}
			}
		}
		pthread_mutex_unlock(&forqueue);
				/*pthread_mutex_lock(&forqueue);
				if(videoQueue.empty()==false)
				{
					send(sock_video,(char*)&videoQueue.front(),1020,0);
					videoQueue.pop();
				}
				pthread_mutex_unlock(&forqueue);*/
	}
	
			//send(sock_video,&frame_packet,sizeof(frame_packet),0);			
}
