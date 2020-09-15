#include <iostream>
#include <caffe/caffe.hpp>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <queue>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <softPwm.h>
#include "Threadfunction.h"

using namespace cv;
using namespace std;
using namespace boost;
using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
pthread_t timer_thr;
void* waspOrbee(void *);
/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class Classifier {
 public:
  Classifier(const string& model_file,
             const string& trained_file,
             const string& mean_file,
             const string& label_file);  //Classifier's Constructor

  std::vector<Prediction> Classify(const cv::Mat& img, int N = 2); //public member method Classify. Classify method get image & classify it.

 private:
  void SetMean(const string& mean_file); //get Mean value from mean file and set it.

  std::vector<float> Predict(const cv::Mat& img); //Predict about image. return value is vector that contains prediction value.

  void WrapInputLayer(std::vector<cv::Mat>* input_channels); //

  void Preprocess(const cv::Mat& img,
                  std::vector<cv::Mat>* input_channels); //

 private:
  boost::shared_ptr<Net<float> > net_; //make CNN
  cv::Size input_geometry_; // input_gemoetry_ is size of input image. it' height will be 224 & width will be 224.
  int num_channels_; //number of channels. We will use RGB channel, so it will be 3.
  cv::Mat mean_; //mean image that made by mean_file. we will subtract it with input image and get new image.
  std::vector<string> labels_; //the labels, we have 2 labels bee & wasp.
};

/*Constructor of Classifier Class*/
Classifier::Classifier(const string& model_file,
                       const string& trained_file,
                       const string& mean_file,
                       const string& label_file) { // The implementation of constructor of Classifier. parameter is model_file's directorty 
						   //& deploy_prototxt' directory & mean_file's directory & label_file' directory
#ifdef CPU_ONLY // we will use caffe at CPU_ONLY
  Caffe::set_mode(Caffe::CPU);
#else
  Caffe::set_mode(Caffe::GPU);
#endif

  /* Load the network. */
  net_.reset(new Net<float>(model_file, TEST));//set deploy.prototxt
  net_->CopyTrainedLayersFrom(trained_file);//set caffe model
  CHECK_EQ(net_->num_inputs(), 1) << "Network should have exactly one input."; //Network input layer must be 1.
  CHECK_EQ(net_->num_outputs(), 1) << "Network should have exactly one output."; //Network output layer musat be 2.


  Blob<float>* input_layer = net_->input_blobs()[0]; // Make Blob<float>* variable named input_layer that get model's first channel
  num_channels_ = input_layer->channels(); // num_channels = input_layer's channel. it will be 3.
  CHECK(num_channels_ == 3 || num_channels_ == 1)
    << "Input layer should have 1 or 3 channels.";
  input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

  /* Load the binaryproto mean file. */
  SetMean(mean_file);

  /* Load labels. */
  std::ifstream labels(label_file.c_str());
  CHECK(labels) << "Unable to open labels file " << label_file;
  string line;
  while (std::getline(labels, line))
    labels_.push_back(string(line));

  Blob<float>* output_layer = net_->output_blobs()[0];
  CHECK_EQ(labels_.size(), output_layer->channels())
    << "Number of labels is different from the output layer dimension."; 
}
/*end of classifier class constructor*/


static bool PairCompare(const std::pair<float, int>& lhs,
                        const std::pair<float, int>& rhs) {
  return lhs.first > rhs.first;
}

/* Return the indices of the top N values of vector v. */
static std::vector<int> Argmax(const std::vector<float>& v, int N) {
  std::vector<std::pair<float, int> > pairs;
  for (size_t i = 0; i < v.size(); ++i)
    pairs.push_back(std::make_pair(v[i], i));
  std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);

  std::vector<int> result;
  for (int i = 0; i < N; ++i)
    result.push_back(pairs[i].second);
  return result;
}

/* Return the top N predictions. */
std::vector<Prediction> Classifier::Classify(const cv::Mat& img, int N) {
  std::vector<float> output = Predict(img);

  N = std::min<int>(labels_.size(), N);
  std::vector<int> maxN = Argmax(output, N);
  std::vector<Prediction> predictions;
  for (int i = 0; i < N; ++i) {
    int idx = maxN[i];
    predictions.push_back(std::make_pair(labels_[idx], output[idx]));
  }

  return predictions;
}

/* Load the mean file in binaryproto format. */
void Classifier::SetMean(const string& mean_file) {
  BlobProto blob_proto;
  ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);

  /* Convert from BlobProto to Blob<float> */
  Blob<float> mean_blob;
  mean_blob.FromProto(blob_proto);
  CHECK_EQ(mean_blob.channels(), num_channels_)
    << "Number of channels of mean file doesn't match input layer.";

  /* The format of the mean file is planar 32-bit float BGR or grayscale. */
  std::vector<cv::Mat> channels;
  float* data = mean_blob.mutable_cpu_data();
  for (int i = 0; i < num_channels_; ++i) {
    /* Extract an individual channel. */
    cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
    channels.push_back(channel);
    data += mean_blob.height() * mean_blob.width();
  }

  /* Merge the separate channels into a single image. */
  cv::Mat mean;
  cv::merge(channels, mean);

  /* Compute the global mean pixel value and create a mean image
   * filled with this value. */
  cv::Scalar channel_mean = cv::mean(mean);
  mean_ = cv::Mat(input_geometry_, mean.type(), channel_mean);
}

std::vector<float> Classifier::Predict(const cv::Mat& img) {
  Blob<float>* input_layer = net_->input_blobs()[0];
  input_layer->Reshape(1, num_channels_,
                       input_geometry_.height, input_geometry_.width);
  /* Forward dimension change to all layers. */
  net_->Reshape();

  std::vector<cv::Mat> input_channels;
  WrapInputLayer(&input_channels);

  Preprocess(img, &input_channels);

  net_->Forward();

  /* Copy the output layer to a std::vector */
  Blob<float>* output_layer = net_->output_blobs()[0];
  const float* begin = output_layer->cpu_data();
  const float* end = begin + output_layer->channels();
  return std::vector<float>(begin, end);
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void Classifier::WrapInputLayer(std::vector<cv::Mat>* input_channels) {
  Blob<float>* input_layer = net_->input_blobs()[0];

  int width = input_layer->width();
  int height = input_layer->height();
  float* input_data = input_layer->mutable_cpu_data();
  for (int i = 0; i < input_layer->channels(); ++i) {
    cv::Mat channel(height, width, CV_32FC1, input_data);
    input_channels->push_back(channel);
    input_data += width * height;
  }
}

void Classifier::Preprocess(const cv::Mat& img,
                            std::vector<cv::Mat>* input_channels) {
  /* Convert the input image to the input image format of the network. */
  cv::Mat sample;
  if (img.channels() == 3 && num_channels_ == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
  else if (img.channels() == 4 && num_channels_ == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
  else if (img.channels() == 4 && num_channels_ == 3)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
  else if (img.channels() == 1 && num_channels_ == 3)
    cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
  else
    sample = img;

  cv::Mat sample_resized;
  if (sample.size() != input_geometry_)
    cv::resize(sample, sample_resized, input_geometry_);
  else
    sample_resized = sample;

  cv::Mat sample_float;
  if (num_channels_ == 3)
    sample_resized.convertTo(sample_float, CV_32FC3);
  else
    sample_resized.convertTo(sample_float, CV_32FC1);

  cv::Mat sample_normalized;
  cv::subtract(sample_float, mean_, sample_normalized);

  /* This operation will write the separate BGR planes directly to the
   * input layer of the network because it is wrapped by the cv::Mat
   * objects in input_channels. */
  cv::split(sample_normalized, *input_channels);

  CHECK(reinterpret_cast<float*>(input_channels->at(0).data)
        == net_->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
}
string model_file   = "/home/pi/model/mobilenet_deploy1.prototxt";
string trained_file = "/home/pi/model/mobilenet_iter_5000.caffemodel";
string mean_file    = "/home/pi/model/mean.binaryproto";
string label_file   = "/home/pi/model/synset.txt";
Classifier classifier(model_file, trained_file, mean_file, label_file);
void* waspOrbee(void *) 
{
	float thresh = 0.9;
	float mean[3] = {0.0};
	int count = 0;
	float mean_value = 0;
	while(1)
	{
		//delay(10);
		if(classifyQueue.empty()==false)
		{

			//std::cout<<"here\n";
		  //string file = "/home/pi/model/checkdata/Wasp2.JPG";

		  //std::cout << "---------- Prediction for "
		  //          << file << " ----------" << std::endl;

		 // cv::Mat img = cv::imread(file, -1);
		//  CHECK(!img.empty()) << "Unable to decode image " << file;
		pthread_mutex_lock(&classifymutex);
		Mat img = classifyQueue.front();
		classifyQueue.pop();
		pthread_mutex_unlock(&classifymutex);
		
		Mat crop;
		crop = img(Range(90,390),Range(124,524));
		//imshow("crop",crop);
		std::vector<Prediction> predictions = classifier.Classify(crop);


	
		
		  /* Print the top N predictions. */
		  for (size_t i = 0; i < predictions.size(); ++i) 
		  {
			Prediction p = predictions[i];
			std::cout << std::fixed << std::setprecision(4) << p.second << " - \""
					 << p.first << "\"" << std::endl;
			 //
		  }
		  if(predictions[0].first == "Wasp" && predictions[0].second>thresh)
		  {
			  printf("in!\n");
			  mean[count] = predictions[0].second;
			  printf("count : %d \n",count);
			  count++;
			  if(count==3)
			  {
				 for(int i = 0; i<3;i++)
				  {
					  mean_value += mean[i];
					  printf("mean[%d] : %f\n",i,mean[i]);
				  }
				  mean_value = mean_value/3;
				  printf("%f\n",mean_value);
				  if(mean_value>0.95)
				  {
					  printf("waspdetected!");
					  ifwaspdetected(timer_thr);
					  mean_value = 0;
					  count = 0;
				  }
				  else
				  {
					  count = 0;
				  }
			  }
		  }
		  else
		  {
			//  printf("else in\n");
			  count = 0;
			  for(int i = 0; i<3;i++)
			  {
				  mean[i] = 0;
			  }
		  }
		  
		  
	  }
	}
	printf("end\n");
}
/*
int main(int argc, char** argv) {
  LOG(FATAL) << "This example requires OpenCV; compile with USE_OPENCV.";
}
*/


