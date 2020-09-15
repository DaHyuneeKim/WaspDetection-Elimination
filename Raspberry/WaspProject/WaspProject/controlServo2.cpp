/*Control Servomotor2 thread - control circuit's movement*/
#include "controlServo.h"

#define PIN_CIRCUIT 2

void * controlServo2_Thread(void *arg)
{
	struct ServoThreadArg* argument;
	int pin;

	argument = (struct ServoThreadArg*)arg;
	pin = argument->Servo_pin;
	//softPwmCreate(pin,0,200);
	while(1)
	{
		if(state ==WASP_DETECTED)
		{
			//printf("enter to main thread\n");
	//		printf("Wasp catch execute Servomotor2\n");
			softServoWrite (pin, -400);
			digitalWrite(PIN_CIRCUIT,HIGH);
			delay(200);
			delay(3000);
			softServoWrite (pin, 750);
			delay(200);
			pthread_mutex_lock(&mutex_lock);//critical section
			state = WASP_NOTDETECTED;
			printf("change to wasp notdetected\n");
			pthread_mutex_unlock(&mutex_lock);//critical section
		}
		if(warning_state==WASP_NOTWARNING&&isalreadywarning==true)
		{
			softServoWrite (pin,1500);
		}
	}
}
