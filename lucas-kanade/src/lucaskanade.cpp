#include <iostream>
#include <string>

#include "opencv4/opencv2/highgui.hpp"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/optflow.hpp"


#include "cluon-complete.hpp"

#include "opendlv-standard-message-set.hpp"
// #include <chrono>
// #include <string>
// #include <vector>

int32_t main(int32_t argc, char **argv)
{
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if ((0 == commandlineArguments.count("image_before")) ||
      (0 == commandlineArguments.count("image_after")) ||
      (0 == commandlineArguments.count("output_flow")) ||
      (0 == commandlineArguments.count("gridstep")) ||
      (0 == commandlineArguments.count("k")) ||
      (0 == commandlineArguments.count("sigma")))
  {
    std::cerr << argv[0] << " estimates the optic flow using lukas-kanade algorithm with pyramidal implementation." << std::endl;
    std::cerr << "Usage:   " << argv[0] << " --image_before=<first image> --image_after=<second image> --output_flow=<flow output> --gridstep=<stride used in sparse match computation>,. --k=<number of nearest-neighbor matches>  --sigma=<parameter defining how fast the weights decrease in the locally-weighted affine fitting>" << std::endl;
    std::cerr << "Example: " << argv[0] << " --image_before=img1.png --image_after=img2.png --output_flow=flow.flo --gridstep=8 --k=128 --sigma=0.05" << std::endl;
    return 1;
  }
  std::string const imageBeforePath = commandlineArguments["image_before"];
  std::string const imageAfterPath = commandlineArguments["image_after"];
  std::string const outputFloPath = commandlineArguments["output_flow"];
  int32_t const gridStep = std::stoi(commandlineArguments["gridstep"]);
  int32_t const k = std::stoi(commandlineArguments["k"]);
  float const sigma = std::stof(commandlineArguments["sigma"]);
  // grid_step	stride used in sparse match computation. Lower values usually result in higher quality but slow down the algorithm.
  // k	number of nearest-neighbor matches considered, when fitting a locally affine model. Lower values can make the algorithm noticeably faster at the cost of some quality degradation.
  // sigma	parameter defining how fast the weights decrease in the locally-weighted affine fitting. Higher values can help preserve fine details, lower values can help to get rid of the noise in the output flow. 

  cv::TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 20, 0.03);

  cv::UMat uImageBefore = cv::imread(imageBeforePath).getUMat(cv::ACCESS_WRITE);
  cv::UMat uImageAfter = cv::imread(imageAfterPath).getUMat(cv::ACCESS_WRITE);

  cv::Mat flowMat;
  cv::optflow::calcOpticalFlowSparseToDense(uImageBefore, uImageAfter, flowMat, gridStep, k, sigma, true, 500.0f, 1.5f);
  cv::optflow::writeOpticalFlow(outputFloPath, flowMat);
  // std::cout << "Done" << std::endl;

  return 0;
}
