#include <iostream>*+
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <caffe/caffe.hpp>
#include <caffe/net.hpp>
#include <string>
#include <iosfwd>
#include <utility>
#include <memory>
#include "video.h"
#include "socketParameter.h"
#include "Threadfunction.h"
#include <time.h>
using namespace std;
using namespace cv;
using namespace caffe;
pthread_mutex_t classifymutex;
/*video_Frame*/ 
queue <cv::Mat> videoQueue;
queue <cv::Mat> classifyQueue;
void changeMattochar(cv::Mat & image);
void * realtimevideo_Thread(void* )
{
	
	raspicam::RaspiCam_Cv Camera;
	Mat image;
	
	
	Camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
	Camera.set(CV_CAP_PROP_FRAME_WIDTH,640);
	Camera.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	
	if(!Camera.open())
	{
		cerr<<"Error opening the camera"<<endl;
		exit(1);
	}
	//int count = 0;
	int old_timeVideo = (int)time(NULL);
	int count = 0;
	while(1)
	{
		Camera.grab();
		Camera.retrieve (image);
		imshow("WaspProject",image);
		int new_timeVideo = (int)time(NULL);
		int fps = new_timeVideo - old_timeVideo;
		if(isconnectedtoserver==true&&count==0)
		{
			pthread_mutex_lock(&forqueue);
			videoQueue.push(image);
			printf("push\n");
			pthread_mutex_unlock(&forqueue);
			classifyQueue.push(image);
			count = count+1;
		}
		if(fps == 3)
		{
			pthread_mutex_lock(&forqueue);
			videoQueue.push(image);
			pthread_mutex_unlock(&forqueue);
			old_timeVideo = (int)time(NULL);
			pthread_mutex_lock(&classifymutex);
			classifyQueue.push(image);

			pthread_mutex_unlock(&classifymutex);
		}
		//printf("Showimage\n");
		
		if(waitKey(20)==27)
		{
			break;
		}
	}
	
	Camera.release();
	exit(1);
}


