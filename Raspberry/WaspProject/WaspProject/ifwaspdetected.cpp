#include "timerthread.h"
#include "detectState.h"
#include "Threadfunction.h"
#include <pthread.h>
void ifwaspdetected(pthread_t timer_thread)
{
	pthread_mutex_lock(&mutex_lock); //Critical Section
	state = WASP_DETECTED;
	pthread_mutex_unlock(&mutex_lock);//End Critical Section
	old_time = (int)time(NULL);	 //if wasp detected, old time is become now.
//	printf("old_time : %d",old_time);
	if(isalreadywarning == false)
	{
		pthread_mutex_lock(&mutex_lock); //Critical Section
		warning_state = WASP_WARNING;
		pthread_mutex_unlock(&mutex_lock);//End Critical Section
		if(pthread_create(&timer_thread,NULL,timer_Thread,NULL))//Create Timer Thread.
		{
			printf("Error while creating timer thread\n");
			return ;
		}
	}
	
}
