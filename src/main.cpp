#include <iostream>

#include <string>
#include <vector>

#include <eigen3/Eigen/Dense>
#include <videostream.hpp>
#include <pso.hpp>
#include <cmath>
#include <opencv2/core/eigen.hpp>

#include <opencv2/imgproc.hpp>

int32_t main(int32_t argc, char **argv)
{
  (void) argc;
  (void) argv;

  // std::string path1 = "/home/bjornborg/test.png";
  std::string path1 = "/home/bjornborg/data/Beanbags/frame07.png";
  std::string path2 = "/home/bjornborg/data/Beanbags/frame08.png";
  VideoStream vStream = VideoStream();
  cv::Mat frame1 = cv::imread(path1, CV_LOAD_IMAGE_COLOR);
  cv::Mat frame2 = cv::imread(path2, CV_LOAD_IMAGE_COLOR);
  vStream.SetCurrentFrame(frame1);
  vStream.Draw();
  Eigen::Vector2i searchWindowSize(4, 4);

  std::vector<Eigen::MatrixXi> eigMat1 = ObjectiveFunction::CvBgr2Eigen(frame1);
  std::vector<Eigen::MatrixXi> eigMat2 = ObjectiveFunction::CvBgr2Eigen(frame2);
  ObjectiveFunction objFun(eigMat1, eigMat2, Eigen::Vector2i(2,2), searchWindowSize);
  
  // double score = ObjectiveFunction::GetScore(eigMat1, eigMat2, Eigen::Vector2i(2,2), Eigen::Vector2i(2,2), searchWindowSize);

  // std::cout << score << std::endl;

  // cv::Size size = frame1.size();
  // int32_t rows = size.height;
  // int32_t cols = size.width;

  // cv::Point initialP(150,145);
  // cv::Scalar color(255, 0, 0);
  // cv::Point arrow(200, 200);
  // cv::Point endP = initialP;
  // endP = endP + cv::Point(searchWindowSize(0),searchWindowSize(1));

  // cv::rectangle(frame1, initialP, endP, color, 0, 0);
  // cv::rectangle(frame2, initialP, endP, color, 0, 0);
  // cv::arrowedLine(frame2, initialP, arrow, color, 1, 1, 0, 0.1);
  // std::cout << frame1.type() << std::endl;
  // std::cout << frameEnd.type() << std::endl;
  // vStream.SetCurrentFrame(frame2);
  // vStream.Draw();






  // cv::Mat test(500, 500, CV_8UC3, cv::Scalar(0, 0, 0));
  
  // Eigen::Vector2i windowSize(600, 1200);
  // Eigen::Vector2d constants;
  // constants << 0.05,0.001;
  // uint32_t numBoids = 30;
  // double visibilityRange = 100;

  // Pso pso = Pso(numBoids, windowSize, visibilityRange, constants, searchWindowSize);

  // cv::Mat map(windowSize(0), windowSize(1), CV_8UC3, cv::Scalar(0, 0, 0));
  // for (int32_t i = 0; i < windowSize(0); i++) {
  //   for (int32_t j = 0; j < windowSize(1); j++) {
  //     Eigen::Vector2i pos(i,j);
  //     map.at<cv::Vec3b>(i,j)[2] =  static_cast<unsigned char>(ObjectiveFunction::GetValue(pos)  / 7);
  //   }
  // }
  // vStream.SetCurrentFrame(map);
  // vStream.Draw();

  // Eigen::IOFormat CleanFmt(0, 0, " ", " ", "", "");
  // for (uint32_t i = 0; i < 1000; i++){
  //   // std::cout << pso.ToString() << std::endl;
  //   // std::cout << "Best: " << pso.GetSwarmBestPerformance() << " at " << pso.GetSwarmBestPosition().format(CleanFmt) << std::endl;
  //   pso.Step();
  //   vStream.SetCurrentFrame(pso.GetFrame(map.clone()));
  //   vStream.Draw();
  // }
  // std::cout << "Best: " << pso.GetSwarmBestPerformance() << " at " << pso.GetSwarmBestPosition().format(CleanFmt) << std::endl;


  return 0;

}
