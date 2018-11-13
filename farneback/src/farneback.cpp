#include <iostream>
#include <string>

#include "opencv4/opencv2/highgui.hpp"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/optflow.hpp"


#include "cluon-complete.hpp"

int32_t main(int32_t argc, char **argv)
{
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if ((0 == commandlineArguments.count("image_before")) ||
      (0 == commandlineArguments.count("image_after")) ||
      (0 == commandlineArguments.count("output_flow")) ||
      (0 == commandlineArguments.count("pyr_scale")) ||
      (0 == commandlineArguments.count("levels")) ||
      (0 == commandlineArguments.count("winsize")) ||
      (0 == commandlineArguments.count("iterations")) ||
      (0 == commandlineArguments.count("poly_n")) ||
      (0 == commandlineArguments.count("poly_sigma")))
  {
    std::cerr << argv[0] << " estimates the optic flow using farnebäck algorithm with pyramidal implementation." << std::endl;
    std::cerr << "Usage:   " << argv[0] 
        << " --image_before=<first image>" 
        << " --image_after=<second image>"
        << " --output_flow=<flow output>"
        << " --pyr_scale=<specifying the image scale (<1) to build pyramids for each image>"
        << " --levels=<number of pyramid layers>"
        << " --winsize=<averaging window size>"
        << " --iterations=<number of iterations the algorithm does at each pyramid level>"
        << " --poly_n=<size of the pixel neighborhood used to find polynomial expansion in each pixel>"
        << " --poly_sigma=<standard deviation of the Gaussian that is used to smooth derivatives>" 
        << std::endl;
    std::cerr << "Example: " << argv[0] 
        << " --image_before=img1.png"
        << " --image_after=img2.png"
        << " --output_flow=flow.flo"
        << " --pyr_scale=0.5"
        << " --levels=3"
        << " --winsize=15"
        << " --iterations=3"
        << " --poly_n=5"
        << " --poly_sigma=1.2" 
        << std::endl;
    return 1;
  }
  std::string const imageBeforePath = commandlineArguments["image_before"];
  std::string const imageAfterPath = commandlineArguments["image_after"];
  std::string const outputFloPath = commandlineArguments["output_flow"];
  double const pyr_scale = std::stod(commandlineArguments["pyr_scale"]);
  int32_t const levels = std::stoi(commandlineArguments["levels"]);
  int32_t const winsize = std::stoi(commandlineArguments["winsize"]);
  int32_t const iterations = std::stoi(commandlineArguments["iterations"]);
  int32_t const poly_n = std::stoi(commandlineArguments["poly_n"]);
  double const poly_sigma = std::stod(commandlineArguments["poly_sigma"]);
  // pyr_scale	parameter, specifying the image scale (<1) to build pyramids for each image; pyr_scale=0.5 means a classical pyramid, where each next layer is twice smaller than the previous one.
  // levels	number of pyramid layers including the initial image; levels=1 means that no extra layers are created and only the original images are used.
  // winsize	averaging window size; larger values increase the algorithm robustness to image noise and give more chances for fast motion detection, but yield more blurred motion field.
  // iterations	number of iterations the algorithm does at each pyramid level.
  // poly_n	size of the pixel neighborhood used to find polynomial expansion in each pixel; larger values mean that the image will be approximated with smoother surfaces, yielding more robust algorithm and more blurred motion field, typically poly_n =5 or 7.
  // poly_sigma	standard deviation of the Gaussian that is used to smooth derivatives used as a basis for the polynomial expansion; for poly_n=5, you can set poly_sigma=1.1, for poly_n=7, a good value would be poly_sigma=1.5.


  cv::UMat uImageBefore = cv::imread(imageBeforePath, cv::IMREAD_GRAYSCALE).getUMat(cv::ACCESS_WRITE);
  cv::UMat uImageAfter = cv::imread(imageAfterPath, cv::IMREAD_GRAYSCALE).getUMat(cv::ACCESS_WRITE);

  cv::Mat flowMat;
  cv::calcOpticalFlowFarneback(uImageBefore, uImageAfter, flowMat, pyr_scale, levels, winsize, iterations, poly_n, poly_sigma, 0);
  cv::writeOpticalFlow(outputFloPath, flowMat);

  return 0;
}
