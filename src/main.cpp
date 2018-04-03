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
#include "opticalflow.hpp"
int32_t main(int32_t argc, char **argv)
{
  (void) argc;
  (void) argv;

  // std::string path1 = "/home/bjornborg/test.png";
  std::string path1 = "/home/bjornborg/data/Beanbags/frame07.png";
  std::string path2 = "/home/bjornborg/data/Beanbags/frame14.png";
  // std::string path3 = "/home/bjornborg/data/Beanbags/frame14.png";
  // VideoStream refStream = VideoStream("Reference");
  // VideoStream searchStream = VideoStream("Search");
  cv::Mat frame1 = cv::imread(path1, CV_LOAD_IMAGE_COLOR);
  cv::Mat frame2 = cv::imread(path2, CV_LOAD_IMAGE_COLOR);
  // cv::Mat frame3 = cv::imread(path3, CV_LOAD_IMAGE_COLOR);
  
  Eigen::Vector2i searchWindowSize(27, 27);

  // auto start = std::chrono::steady_clock::now();
  // auto end = std::chrono::steady_clock::now();

  Eigen::Vector2d constants(0.1,0.05);
  Eigen::Vector2i origin(165,170);
  std::vector<Eigen::Vector2i> originVec;
  originVec.push_back(origin);
  uint32_t numBoids = 15;
  OpticalFlow of(frame1, frame2, numBoids, originVec, searchWindowSize, constants);
  of.GetOpticalFlow();
  of.DrawOpticalFlow();

  // Pso pso(eigMat1, eigMat2, origin, numBoids, constants, searchWindowSize); 
  // pso.SetCurrentFrame(eigMat3);
  // cv::Point initialP(origin(1)-searchWindowSize(0)/2,origin(0)-searchWindowSize(1)/2);
  // cv::Scalar color(255, 0, 0);
  // // cv::Point arrow(200, 200);
  // cv::Point endP = initialP;
  // endP = endP + cv::Point(searchWindowSize(0),searchWindowSize(1));
  // cv::rectangle(frame1, initialP, endP, color, 0, 0);
  // refStream.SetCurrentFrame(frame1);
  // refStream.Draw();
  // // cv::waitKey();



  // Eigen::IOFormat CleanFmt(0, 0, " ", " ", "", "");
  // pso.Step();
  // std::chrono::duration<double, std::milli> diff = end-start;
  // std::cout << "Time: " << diff.count() << " ms, " << std::endl;
  // for (uint32_t i = 0; i < 500; i++){
  //   std::cout << pso.ToString() << std::endl;
  //   pso.Step();
  //   Eigen::Vector2i pos = pso.GetSwarmBestPosition();
  //   cv::Point p0(pos(1)-searchWindowSize(0)/2,pos(0)-searchWindowSize(1)/2);
  //   cv::Point p1 = p0;
  //   p1 = p1 + cv::Point(searchWindowSize(0),searchWindowSize(1));
  //   cv::Mat searchMat = pso.GetFrame(frame3.clone());
  //   cv::rectangle(searchMat, p0, p1, color, 0, 0);
  //   searchStream.SetCurrentFrame(searchMat);
  //   searchStream.Draw();
  //   std::cout << "Best: " << pso.GetSwarmBestPerformance() << " at " << pso.GetFlowVector().format(CleanFmt) << std::endl;
  //   cv::waitKey();
  // }
  // // cv::waitKey();

  return 0;

}
