/*#include <iostream>
using namespace std;
#include <string.h>
#include <stdlib.h>
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "detectState.h"
#include <arpa/inet.h>
#include <wiringPi.h>
#include "socketParameter.h"
#include <unistd.h>

void* sendTemp_Thread(void*)
{

	while(1)
	{
		send(sock,(char*)&temp_String,sizeof(tempString),0);
		delay(3000);
	}
	close(sock);

}

*/
