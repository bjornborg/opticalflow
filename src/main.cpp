#include <iostream>

#include <string>
#include <vector>

#include <eigen3/Eigen/Dense>
#include <videostream.hpp>
#include <pso.hpp>

int32_t main(int32_t argc, char **argv)
{
  (void) argc;
  (void) argv;

  VideoStream vStream = VideoStream();
  // cv::Mat test(500, 500, CV_8UC3, cv::Scalar(0, 0, 0));
  
  Eigen::Vector2i windowSize(500, 600);

  Pso pso = Pso(1, windowSize);

  pso.ToString();
  for (uint32_t i = 0; i < 100; i++){
    pso.Step();
    vStream.SetCurrentFrame(pso.GetFrame());
    vStream.Draw();
  }


  // std::string dataSet = "/home/bjornborg/data/DogDance/frame%02d.png";
  // Boid p = Boid();
  // std::cout << p.ToString() << std::endl;
  // p.SetAcceleration(Eigen::Vector2i(1,-1));
  // std::cout << p.ToString() << std::endl;
  // p.Step();
  // std::cout << p.ToString() << std::endl;
  // p.Step();
  // std::cout << p.ToString() << std::endl;
  // p.Step();
  // std::cout << p.ToString() << std::endl;
  

  // VideoStream stream(dataSet);
  // stream.NextFrame();
  // while (stream.NextFrame()) {
  //   stream.Draw();
  // }


  // std::cout << "Played frames: " << stream.GetFrameCounter()-1 << std::endl; 


  return 0;

}
