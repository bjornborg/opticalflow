#include <iostream>
#include <string>

// #include "opencv4/opencv2/highgui.hpp"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/optflow.hpp"
// #include "opencv4/opencv2/core/utility.hpp"
#include "opencv4/opencv2/imgcodecs.hpp"
#include "opencv4/opencv2/video.hpp"

#include "cluon-complete.hpp"

int32_t main(int32_t argc, char **argv) {
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if ((0 == commandlineArguments.count("image_before")) ||
      (0 == commandlineArguments.count("image_after")) ||
      (0 == commandlineArguments.count("output_flow"))) {
    std::cerr << argv[0]
              << " estimates the optic flow using Deepflow algorithm ."
              << std::endl;
    std::cerr << "Usage:   " << argv[0] << " --image_before=<first image>"
              << " --image_after=<second image>"
              << " --output_flow=<flow output>\n"
              << std::endl;
    std::cerr << "Example: " << argv[0] << " --image_before=img1.png"
              << " --image_after=img2.png"
              << " --output_flow=flow.flo" << std::endl;
    return 1;
  }
  std::string const imageBeforePath = commandlineArguments["image_before"];
  std::string const imageAfterPath = commandlineArguments["image_after"];
  std::string const outputFloPath = commandlineArguments["output_flow"];

  cv::UMat uImageBefore = cv::imread(imageBeforePath, cv::IMREAD_GRAYSCALE)
                              .getUMat(cv::ACCESS_WRITE);
  cv::UMat uImageAfter = cv::imread(imageAfterPath, cv::IMREAD_GRAYSCALE)
                             .getUMat(cv::ACCESS_WRITE);

  cv::UMat flowMat;
  cv::Ptr<cv::DenseOpticalFlow> deepflow =
      cv::optflow::createOptFlow_DeepFlow();

  deepflow->calc(uImageBefore, uImageAfter, flowMat);
  cv::writeOpticalFlow(outputFloPath, flowMat);

  return 0;
}
