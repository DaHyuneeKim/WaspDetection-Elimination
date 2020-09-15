#include <iostream>
using namespace std;
#include "timerthread.h"
#include "detectState.h"
void * timer_Thread(void * arg)
{
	printf("Create timer thread because warning state become WASP_WARNING!\n");
	int new_time = (int)time(NULL);
	int old_executeTime = new_time-old_time;
	while(old_executeTime !=20 )//stop thread after 20seconds
	{
		new_time = (int)time(NULL);
		int new_executeTime = new_time-old_time;
	//	printf("new_exwecuteTime : %d\n",new_executeTime);
		delay(20);
		if(old_executeTime!=new_executeTime)
		{
			old_executeTime = new_executeTime;
			if(old_executeTime==0)
			{
				printf("new Wasp Detected!!! Reset timer!\n\n");
			}
			printf("Remaining time until warning is lifted: %d seconds\n",20-(old_executeTime));
		}
	}
	printf("End timer Thread. Open entrance and release WASP WARNING\n");
	pthread_mutex_lock(&mutex_lock); //Critical Section
	warning_state = WASP_NOTWARNING;
	pthread_mutex_unlock(&mutex_lock);//End Critical Section
}
