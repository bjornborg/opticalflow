#include <iostream>
#include <chrono>
#include <string>
#include <vector>

#include <eigen3/Eigen/Dense>
#include <cmath>
#include <opencv2/core/eigen.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp> 
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include "opticalflow.hpp"
int32_t main(int32_t argc, char **argv)
{
  (void) argc;
  (void) argv;
  Eigen::initParallel();

  // std::string path = "/home/bjornborg/Video/Sample.mp4";


  // std::string path1 = "/home/bjornborg/data/Beanbags/frame07.png";
  // std::string path2 = "/home/bjornborg/data/Beanbags/frame14.png";
  // std::string path3 = "/home/bjornborg/data/Beanbags/frame14.png";
  // VideoStream refStream = VideoStream("Reference");
  // VideoStream searchStream = VideoStream("Search");
  // cv::Mat frame1 = cv::imread(path1, CV_LOAD_IMAGE_COLOR);
  // cv::Mat temp = frame1.clone();
  // cv::flip(temp, frame1, 1);
  // cv::Mat frame2 = cv::imread(path2, CV_LOAD_IMAGE_COLOR);
  // temp = frame2.clone();
  // cv::flip(temp, frame2, 1);
  // cv::Mat frame3 = cv::imread(path3, CV_LOAD_IMAGE_COLOR);
  
  cv::VideoCapture cap(0); // open the default camera
  if(!cap.isOpened()){
    std::cout << "nope" << std::endl;
    return -1;
  }  // check if we succeeded

  cv::UMat frame0;
  cap >> frame0; 
  cv::UMat frame1;
  cap >> frame1; 

  Eigen::Vector2i searchWindowSize(31, 31);


  Eigen::Vector2d constants(0.1,0.05);
  std::vector<Eigen::Vector2i> originVec;
  originVec.push_back(Eigen::Vector2i(465,70));
  originVec.push_back(Eigen::Vector2i(365,70));
  originVec.push_back(Eigen::Vector2i(265,70));
  originVec.push_back(Eigen::Vector2i(165,70));
  originVec.push_back(Eigen::Vector2i(465,170));
  originVec.push_back(Eigen::Vector2i(365,170));
  originVec.push_back(Eigen::Vector2i(265,170));
  originVec.push_back(Eigen::Vector2i(165,170));
  originVec.push_back(Eigen::Vector2i(465,270));
  originVec.push_back(Eigen::Vector2i(365,270));
  originVec.push_back(Eigen::Vector2i(265,270));
  originVec.push_back(Eigen::Vector2i(165,270));
  originVec.push_back(Eigen::Vector2i(465,370));
  originVec.push_back(Eigen::Vector2i(365,370));
  originVec.push_back(Eigen::Vector2i(265,370));
  originVec.push_back(Eigen::Vector2i(165,370));
  uint32_t numBoids = 10;
  OpticalFlow of(frame0, frame1, numBoids, originVec, searchWindowSize, constants);
  cv::UMat temp;
  auto end = std::chrono::steady_clock::now();
  auto start = std::chrono::steady_clock::now();
  while(cap.read(temp))
  {

    of.SetNewFrame(temp);
    of.GetOpticalFlow();
    of.DrawOpticalFlow();
    // cv::waitKey();
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> diff = end-start;
    std::cout << "Time: " << diff.count() << " ms, " << std::endl;
    start = std::chrono::steady_clock::now();
  }
  return 0;
}