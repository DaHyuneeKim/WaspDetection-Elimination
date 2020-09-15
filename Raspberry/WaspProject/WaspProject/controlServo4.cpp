/*Control Servomotor4 thread - control entrance */
#include "controlServo.h"

void * controlServo4_Thread(void *arg)
{
	struct ServoThreadArg* argument;
	int pin;

	argument = (struct ServoThreadArg*)arg;
	pin = argument->Servo_pin;
	//softPwmCreate(pin,0,200);
	while(1)
	{
		delay(20);
		if(warning_state == WASP_WARNING&&isalreadywarning==false)
		{
			printf("Close entrance of hive. If wasp didn't detected at camera while after 20seconds, entrance will be opened.\n");
			softServoWrite (pin,1500); //close entrance
			pthread_mutex_lock(&mutex_lock); //Critical Section
			isalreadywarning = true;
			pthread_mutex_unlock(&mutex_lock); //Critical Section
		}
		if(warning_state==WASP_NOTWARNING&&isalreadywarning==true)
		{
			softServoWrite(pin,450);//open entrance
			delay(500);
			pthread_mutex_lock(&mutex_lock); //Critical Section
			isalreadywarning = false;
			pthread_mutex_unlock(&mutex_lock); //Critical Section
		}
	}
	
}
