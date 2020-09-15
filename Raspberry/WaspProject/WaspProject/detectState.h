#ifndef _DETECTSTATE_H_
#define _DETECTSTATE_H_
#include <iostream>
using namespace std;
#include <wiringPi.h>
#include <pthread.h>
enum wasp_detectedstate{WASP_NOTDETECTED, WASP_DETECTED};
enum wasp_warningstate{WASP_WARNING,WASP_NOTWARNING};
extern int state;
extern int warning_state;
extern bool isalreadywarning;
extern pthread_mutex_t mutex_lock;
#endif
