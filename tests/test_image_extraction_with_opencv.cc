#define DEBUG

//this test is intended to show the performance in terms of time of Opencv to extract a single jpg image from an mp4 video
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "Utils.hh"
#include <boost/core/ignore_unused.hpp>

int main(int argc, char **argv) {
  boost::ignore_unused(argc);
  boost::ignore_unused(argv);

  Time_spent<> time_spent{};
  cv::VideoCapture cap("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-06-24_14:39:34_cam02_5.mp4"); // video
  if ( !cap.isOpened() ){
    std::cerr << "Cannot open the video file" << std::endl;
    return 1;
  }

  double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
  D( std::cout << "Frame per seconds : " << fps << std::endl );

  cv::Mat frame;
  bool bSuccess = cap.read(frame); // read a new frame from video
  
  if (!bSuccess){
    std::cerr << "Cannot read the frame from video file" << std::endl;
    return 1;
  }
  imwrite("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-06-24_14:39:34_cam02_5_opencv_start.jpg", frame);

  //we can set a specific time in the video to extract the image from
  cap.set(CV_CAP_PROP_POS_MSEC, 2800);
  bSuccess = cap.read(frame);

  if (!bSuccess){
    std::cerr << "Cannot read the frame from video file" << std::endl;
    return 1;
  }

  imwrite("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-06-24_14:39:34_cam02_5_opencv_end.jpg", frame);

  std::vector<int> jpeg_params;
  jpeg_params.push_back(CV_IMWRITE_JPEG_QUALITY);
  jpeg_params.push_back(65);
      
  imwrite("/home/pi/mqtt/mqtt/temp_saved_file_dir/2019-06-24_14:39:34_cam02_5_opencv_lower.jpg", frame, jpeg_params);
  return 0;
  
}
