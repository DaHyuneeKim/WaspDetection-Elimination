#ifndef _THREADFUNCTION_H_
#define _THREADFUNCTION_H_
#include <queue>
#include <pthread.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
extern void * controlServo1_Thread(void *arg);
extern void * controlServo2_Thread(void *arg);
extern void * controlServo3_Thread(void *arg);
extern void * controlServo4_Thread(void *arg);
extern void ifwaspdetected(pthread_t timer_thread);
extern void * timer_Thread(void * arg);
extern void * readTemperature_Thread(void* );
extern void* sendTemp_Thread(void*);
extern void* receiveControl_thread(void*);
extern void * send_VideoThread(void* );
extern void * realtimevideo_Thread(void* );
extern void* waspOrbee(void *);
extern pthread_mutex_t classifymutex;
extern queue <cv::Mat> classifyQueue;
#endif
