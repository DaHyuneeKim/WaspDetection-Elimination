/*Control Servomotor3 thread - - control bar that use for put away a dead body of Wasp after kill it */
#include "controlServo.h"

void * controlServo3_Thread(void *arg)
{
	struct ServoThreadArg* argument;
	int pin;

	argument = (struct ServoThreadArg*)arg;
	pin = argument->Servo_pin;
	//softPwmCreate(pin,0,200);
	
	while(1)
	{
		delay(50);
		if(state ==WASP_DETECTED)
		{
		//	printf("enter to main thread\n");
		//printf("enter thread servo3\n");
			delay(4000);
			softServoWrite (pin, -400);
			delay(500);
			softServoWrite (pin,1500);
			delay(500);
		}
	}
}
