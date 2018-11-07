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
      (0 == commandlineArguments.count("layers")) ||
      (0 == commandlineArguments.count("averaging_block_size")) ||
      (0 == commandlineArguments.count("max_flow")) ||
      (0 == commandlineArguments.count("sigma_dist")) ||
      (0 == commandlineArguments.count("sigma_color")) ||
      (0 == commandlineArguments.count("occ_thr")) ||
      (0 == commandlineArguments.count("upscale_averaging_radius")) ||
      (0 == commandlineArguments.count("upscale_sigma_dist")) ||
      (0 == commandlineArguments.count("upscale_sigma_color")) ||
      (0 == commandlineArguments.count("speed_up_thr")))
  {
    std::cerr << argv[0] << " estimates the optic flow using simpleflow algorithm." << std::endl;
    std::cerr << "Usage:   " << argv[0]
              << " --image_before=<first image>" << std::endl
              << " --image_after=<second image>" << std::endl
              << " --output_flow=<flow output>" << std::endl
              << " --layers=<Number of layers>" << std::endl
              << " --averaging_block_size=<Size of block through which we sum up when calculate cost function for pixel>" << std::endl
              << " --max_flow=<maximal flow that we search at each level>" << std::endl
              << " --sigma_dist=<vector smooth spatial sigma parameter>" << std::endl
              << " --sigma_color=<vector smooth color sigma parameter>" << std::endl
              << " --postprocess_window=<window size for postprocess cross bilateral filter>" << std::endl
              << " --sigma_dist_fix=<spatial sigma for postprocess cross bilateral filter>" << std::endl
              << " --sigma_color_fix=<color sigma for postprocess cross bilateral filter>" << std::endl
              << " --occ_thr=<threshold for detecting occlusions>" << std::endl
              << " --upscale_averaging_radius=<window size for bilateral upscale operation>" << std::endl
              << " --upscale_sigma_dist=<spatial sigma for bilateral upscale operation>" << std::endl
              << " --upscale_sigma_color=<color sigma for bilateral upscale operation>" << std::endl
              << " --speed_up_thr=<threshold to detect point with irregular flow - where flow should be recalculated after upscale>" << std::endl
              << std::endl;
    std::cerr << "Example: " << argv[0]
              << " --image_before=img1.png"
              << " --image_after=img2.png"
              << " --output_flow=flow.flo"
              << " --layers=3"
              << " --averaging_block_size=2"
              << " --max_flow=4"
              << " --sigma_dist=4.1"
              << " --sigma_color=25.5"
              << " --postprocess_window=18"
              << " --sigma_dist_fix=55.0"
              << " --sigma_color_fix=25.5"
              << " --occ_thr=0.35"
              << " --upscale_averaging_radius=18"
              << " --upscale_sigma_dist=55.0"
              << " --upscale_sigma_color=25.5"
              << " --speed_up_thr=10.0"
              << std::endl;
    return 1;
  }

  std::string const imageBeforePath = commandlineArguments["image_before"];
  std::string const imageAfterPath = commandlineArguments["image_after"];
  std::string const outputFloPath = commandlineArguments["output_flow"];
  int32_t const layers = std::stoi(commandlineArguments["layers"]);
  int32_t const averaging_block_size = std::stoi(commandlineArguments["averaging_block_size"]);
  int32_t const max_flow = std::stoi(commandlineArguments["max_flow"]);
  double const sigma_dist = std::stod(commandlineArguments["sigma_dist"]);
  double const sigma_color = std::stod(commandlineArguments["sigma_color"]);
  int32_t const postprocess_window = std::stoi(commandlineArguments["postprocess_window"]);
  double const sigma_dist_fix = std::stod(commandlineArguments["sigma_dist_fix"]);
  double const sigma_color_fix = std::stod(commandlineArguments["sigma_color_fix"]);
  double const occ_thr = std::stod(commandlineArguments["occ_thr"]);
  int32_t const upscale_averaging_radius = std::stoi(commandlineArguments["upscale_averaging_radius"]);
  double const upscale_sigma_dist = std::stod(commandlineArguments["upscale_sigma_dist"]);
  double const upscale_sigma_color = std::stod(commandlineArguments["upscale_sigma_color"]);
  double const speed_up_th = std::stod(commandlineArguments["speed_up_thr"]);

  // layers	Number of layers
  // averaging_block_size	Size of block through which we sum up when calculate cost function for pixel
  // max_flow	maximal flow that we search at each level
  // sigma_dist	vector smooth spatial sigma parameter
  // sigma_color	vector smooth color sigma parameter
  // postprocess_window	window size for postprocess cross bilateral filter
  // sigma_dist_fix	spatial sigma for postprocess cross bilateral filter
  // sigma_color_fix	color sigma for postprocess cross bilateral filter
  // occ_thr	threshold for detecting occlusions
  // upscale_averaging_radius	window size for bilateral upscale operation
  // upscale_sigma_dist	spatial sigma for bilateral upscale operation
  // upscale_sigma_color	color sigma for bilateral upscale operation
  // speed_up_thr	threshold to detect point with irregular flow - where flow should be recalculated after upscale

  cv::UMat uImageBefore = cv::imread(imageBeforePath).getUMat(cv::ACCESS_WRITE);
  cv::UMat uImageAfter = cv::imread(imageAfterPath).getUMat(cv::ACCESS_WRITE);

  cv::Mat flowMat;
  cv::optflow::calcOpticalFlowSF(uImageBefore, uImageAfter, flowMat, layers, averaging_block_size, max_flow, sigma_dist, sigma_color, postprocess_window, sigma_dist_fix, sigma_color_fix, occ_thr, upscale_averaging_radius, upscale_sigma_dist, upscale_sigma_color, speed_up_th);
  cv::optflow::writeOpticalFlow(outputFloPath, flowMat);

  return 0;
}
