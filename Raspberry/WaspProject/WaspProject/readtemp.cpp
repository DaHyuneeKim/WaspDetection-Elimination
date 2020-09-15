/* Read temperature thread */
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <wiringPi.h>
#include "socketParameter.h"


#define PROBEPATH    "/sys/bus/w1/devices"      // Location of the probes
#define MAXPROBES    5                          // Max number of probes
#define PROBENAMELEN 80                         // Max length of the probenames including directory.
#define BUFSIZE      256                        // Typical charbuffer

int findprobes(void);

char probepath[MAXPROBES][PROBENAMELEN];
char probename[MAXPROBES][PROBENAMELEN];
char alias[MAXPROBES][BUFSIZE];
FILE *probefd;
int numOfSensor;
void * readTemperature_Thread(void* )
{
	       int i;
	    double temperature;
        char *temp;
        char buf[BUFSIZE];
        char temp_String[8];
 
        numOfSensor = findprobes(); //몇개의 센서가 연결되어 있는지 확인하고 그 위치를 저장한다.
        if (numOfSensor == 0)
        {
                printf("Error: No DS18B20 compatible probes located.\n");
                exit(-1);
        }
 
 
        while(1)
        {
			delay(20);
                for (i = 0; i < numOfSensor; i++) //발견된 센서수 만큼 반복한다.
                {
                        probefd = fopen(probepath[i], "r"); //probepath에 저장된 위치로부터 센서값을 읽어올 수 있다.
 
                        if (probefd == NULL)
                        {
                                printf( "Error: Unable to open '%s': %s\n",
                                                probepath[i], strerror(errno));
                                exit(-1);
                        }
 
                        fgets(buf, sizeof(buf)-1, probefd); // 첫번째 줄은 무시 함
                        memset(buf, 0, sizeof(buf));
 
                        fgets(buf, sizeof(buf)-1, probefd); //두번째 줄을 불러와서
                        temp = strtok(buf, "t="); //"t="뒤에 오는 온도값을 파싱 한다.
                        temp = strtok(NULL, "t=");
                        temperature = atof(temp)/1000; //1000으로 나누어야 원하 는 온도값이 됨
                        temp_String[0] = (char)((int)temperature/10)+48;
                        temp_String[1] = (char)((int)temperature%10)+48;
                        temp_String[2] = '.';

                        temp_String[3] = (char)((int)(temperature*10)%((temp_String[0]-48)*100+(temp_String[1]-48)*10))+48;
                      //  printf("%d\n",(temp_String[0]-48)*1000+(temp_String[1]-48)*100+(temp_String[3]-48)*10);
                      //  printf("%d\n",(char)((int)(temperature*100)%((temp_String[0]-48)*1000+(temp_String[1]-48)*100+(temp_String[3]-48)*10))+48);
                        temp_String[4] = (char)((int)(temperature*100)%((temp_String[0]-48)*1000+(temp_String[1]-48)*100+(temp_String[3]-48)*10))+48;
                        temp_String[5] = (char)((int)(temperature*1000)%((temp_String[0]-48)*10000+(temp_String[1]-48)*1000+(temp_String[3]-48)*100+(temp_String[4]-48)*10))+48;
                        temp_String[6] = '\r';
                        temp_String[7] = '\n';
                        if(isSocketConnect==true)
                        {
							send(sock,(char*)&temp_String,sizeof(temp_String),0);
						}
                        delay(5000);
 
                        fclose(probefd);
 
 
                }
        }
}

int findprobes(void)
{
        struct dirent *pDirent;
        DIR *pDir;
        int count;
 
        count = 0;
 
        pDir = opendir(PROBEPATH);
        if (pDir == NULL) {
                printf("Cannot open directory '%s'\n", PROBEPATH);
                return 0;
        }
 
        while ((pDirent = readdir(pDir)) != NULL)
        {
                //모든 DS18B20의 아이디는 28-로 시작한다.
                if (pDirent->d_name[0] == '2' && pDirent->d_name[1] == '8' && pDirent->d_name[2] == '-')
                {
					snprintf(probepath[count], PROBENAMELEN-1, "%s/%s/w1_slave", PROBEPATH, pDirent->d_name);
					snprintf(probename[count], PROBENAMELEN-1, "%s", pDirent->d_name);
 
               //         printf ("Found DS18B20 compatible probe named '%s':\nDevice file '%s'\n",probename[count], probepath[count]);
                        count++;
                }
 
        }
 
        closedir(pDir);
 
        return count;
}
