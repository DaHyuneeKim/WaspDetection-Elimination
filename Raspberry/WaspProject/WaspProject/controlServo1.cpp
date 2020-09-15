/*Control Servomotor1 thread - control circuit's movment*/
#include "controlServo.h"
#define PIN_CIRCUIT 2
void * controlServo1_Thread(void *arg)
{
	struct ServoThreadArg* argument;
	int pin;

	argument = (struct ServoThreadArg*)arg;
	pin = argument->Servo_pin;
	//softPwmCreate(pin,0,200);
	while(1)
	{
		//printf("state : %d\n",state);
		if(state ==WASP_DETECTED)
		{
		//	printf("enter to thread\n");
	//	printf("Wasp catch execute Servomotor1\n");
			softServoWrite (pin, 1500);
			delay(200);
			delay(3000);
			digitalWrite(PIN_CIRCUIT,LOW);
			softServoWrite (pin,450);
			delay(220);
		}
		if(warning_state==WASP_NOTWARNING&&isalreadywarning==true)
		{
			softServoWrite (pin,-400);
		}
	}
}
