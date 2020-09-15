#include <iostream>
using namespace std;
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "softServo.h"
#include "ServoThreadArg.h"
#include "Threadfunction.h"
#include "detectState.h"
#include "timerthread.h"

#define PIN_SERVO1 1
#define PIN_SERVO2 24
#define PIN_SERVO3 3
#define PIN_SERVO4 23
#define PIN_TEMPSENSOR 0
#define PIN_CIRCUIT 2

pthread_mutex_t mutex_lock;
int state = WASP_NOTDETECTED;
int warning_state = WASP_NOTWARNING;
int old_time = (int)time(NULL);
bool isalreadywarning = false;
bool isentranceClosed = false;

int  main()
{
	/*Initialize Variables*/
	// main_newTime;
	//int main_oldTime;
	
	/*Initialize RaspberryPi GPIO */
	if(wiringPiSetup() == -1)
	{
		printf("Cannot setup GPIO\nEnd Program\n");
		return  1;
	}
	pinMode(PIN_SERVO1,PWM_OUTPUT);
	pinMode(PIN_SERVO2,PWM_OUTPUT);
	pinMode(PIN_SERVO3,PWM_OUTPUT);
	pinMode(PIN_SERVO4,PWM_OUTPUT);
	pinMode(PIN_CIRCUIT,OUTPUT);
	
	softServoSetup(PIN_SERVO1,PIN_SERVO2,PIN_SERVO3,PIN_SERVO4,-1,-1,-1,-1);
	softServoWrite(PIN_SERVO1,-400);
	softServoWrite(PIN_SERVO2,1500);
	softServoWrite(PIN_SERVO3,1500);
	softServoWrite(PIN_SERVO4,450);
	delay(200);
	printf("Execute Program!\n\n");
	
	/*Initialize thread arguments */
	pthread_t servo_threads[4];
	pthread_t timer_thread;
	pthread_t read_temper;
	pthread_t send_temper;
	pthread_t receive_control;
	pthread_t send_video;
	pthread_t realtime_video;
	pthread_t  classify1;
	struct ServoThreadArg *argServo1;
	struct ServoThreadArg *argServo2;
	struct ServoThreadArg *argServo3;
	struct ServoThreadArg *argServo4;
	argServo1 = (struct ServoThreadArg *)malloc(sizeof(struct ServoThreadArg));
	argServo1->Servo_pin = PIN_SERVO1;
	argServo2 = (struct ServoThreadArg*)malloc(sizeof(struct ServoThreadArg));
	argServo2->Servo_pin = PIN_SERVO2;
	argServo3 = (struct ServoThreadArg*)malloc(sizeof(struct ServoThreadArg));
	argServo3->Servo_pin = PIN_SERVO3;
	argServo4 = (struct ServoThreadArg*)malloc(sizeof(struct ServoThreadArg));
	argServo4->Servo_pin = PIN_SERVO4;
	
	/*Create Thread*/
	if(pthread_create(&servo_threads[0],NULL,controlServo1_Thread,(void *)argServo1)) //Create controlServo1_Thread 
	{
		printf("Error while creating thread\n");
		return -1; 
	}
	if(pthread_create(&servo_threads[1],NULL,controlServo2_Thread,(void *)argServo2))//Create controlServo2_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}
	if(pthread_create(&servo_threads[2],NULL,controlServo3_Thread,(void *)argServo3))//Create controlServo3_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}
	if(pthread_create(&servo_threads[3],NULL,controlServo4_Thread,(void *)argServo4))//Create controlServo4_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}
	if(pthread_create(&read_temper,NULL,readTemperature_Thread,NULL))//Create ReadTemperature_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}
	/*if(pthread_create(&send_temper,NULL,sendTemp_Thread,NULL))//Create ReadTemperature_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}*/
	if(pthread_create(&receive_control,NULL,receiveControl_thread,NULL))//Create ReadTemperature_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}
	if(pthread_create(&send_video,NULL,send_VideoThread,NULL))//Create ReadTemperature_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}
	if(pthread_create(&realtime_video,NULL,realtimevideo_Thread,NULL))//Create ReadTemperature_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}
	if(pthread_create(&classify1,NULL,waspOrbee,NULL))//Create ReadTemperature_Thread
	{
		printf("Error while creating thread\n");
		return -1;
	}


	/*main thread loop*/
	
	/*main_oldTime = (int)time(NULL);
	int old_executemainTime = 0;
	while(1)
	{
		main_newTime = (int)time(NULL);
		int new_executmainbTime= main_newTime - main_oldTime;
		//printf("count: %d\n",count)
		//printf("%d\n",count);
		if(old_executemainTime!=new_executmainbTime)
		{
			old_executemainTime =  new_executmainbTime;
			if( new_executmainbTime ==5)
			{
				printf("\nWASP DETECTED\n");
				ifwaspdetected(timer_thread);
				delay(500);
			}
			if(new_executmainbTime==15)
			{
				printf("\nWASP DETECTED\n");
				ifwaspdetected(timer_thread);
				delay(500);
			}
					
			if(new_executmainbTime==40)
			{
				return 0;
			}
		}
	}*/
	
	while(1)
	{
		
	}
	return 0;
}

