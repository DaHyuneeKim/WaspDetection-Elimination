#include <iostream>
#include <queue>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
using namespace std;
#ifndef _VIDEO_H_
#define _VIDEO_H_
typedef struct packet
{
	char frame[1020];
}video_Frame;

//extern queue <video_Frame> videoQueue;
extern queue <cv::Mat> videoQueue;
extern pthread_mutex_t forqueue;
#endif
