#include <iostream>
#include <string>

// #include "opencv4/opencv2/highgui.hpp"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/optflow.hpp"
// #include "opencv4/opencv2/core/utility.hpp"
#include "opencv4/opencv2/video.hpp"
#include "opencv4/opencv2/imgcodecs.hpp"


#include "cluon-complete.hpp"

int32_t main(int32_t argc, char **argv)
{
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if ((0 == commandlineArguments.count("image_before")) ||
      (0 == commandlineArguments.count("image_after")) ||
      (0 == commandlineArguments.count("tau")) ||
      (0 == commandlineArguments.count("lambda")) ||
      (0 == commandlineArguments.count("theta")) ||
      (0 == commandlineArguments.count("nscales")) ||
      (0 == commandlineArguments.count("warps")) ||
      (0 == commandlineArguments.count("epsilon")) ||
      (0 == commandlineArguments.count("innnerIterations")) ||
      (0 == commandlineArguments.count("outerIterations")) ||
      (0 == commandlineArguments.count("scaleStep")) ||
      (0 == commandlineArguments.count("gamma")) ||
      (0 == commandlineArguments.count("medianFiltering")))
  {
    std::cerr << argv[0] << " estimates the optic flow using dualtvl1 algorithm ." << std::endl;
    std::cerr << "Usage:   " << argv[0] 
        << " --image_before=<first image>" 
        << " --image_after=<second image>"
        << " --output_flow=<flow output>\n"
        << " --tau=<Time step of the numerical scheme>\n"
        << " --lambda=<Weight parameter for the data term, attachment parameter>\n"
        << " --theta=<Weight parameter for (u - v)\\^2, tightness parameter>\n"
        << " --nscales=<Number of scales used to create the pyramid of images>\n"
        << " --warps=<Number of warpings per scale>\n"
        << " --epsilon=<Stopping criterion threshold used in the numerical scheme>\n"
        << " --innnerIterations=<Stopping criterion iterations number used in the numerical schem>\n"
        << " --outerIterations=<Stopping criterion iterations number used in the numerical schem>\n"
        << " --scaleStep=<Stepping size for pyramid scheme>\n"
        << " --gamma=<coefficient for additional illumination variation term>\n"
        << " --medianFiltering=<Median filter kernel size (1 = no filter) (3 or 5)>\n"
        << std::endl;


    std::cerr << "Example: " << argv[0] 
        << " --image_before=img1.png"
        << " --image_after=img2.png"
        << " --output_flow=flow.flo\n"
        << " --tau=0.25\n"
        << " --lambda=0.15\n"
        << " --theta=0.3\n"
        << " --nscales=5\n"
        << " --warps=5\n"
        << " --epsilon=0.01\n"
        << " --innnerIterations=30\n"
        << " --outerIterations=10\n"
        << " --scaleStep=0.8\n"
        << " --gamma=0.0\n"
        << " --medianFiltering=5\n"
        << std::endl;
    return 1;
  }
  std::string const imageBeforePath = commandlineArguments["image_before"];
  std::string const imageAfterPath = commandlineArguments["image_after"];
  std::string const outputFloPath = commandlineArguments["output_flow"];
  std::string const presetStr = commandlineArguments["preset"];
  double const tau = std::stod(commandlineArguments["tau"]);
  double const lambda = std::stod(commandlineArguments["lambda"]);
  double const theta = std::stod(commandlineArguments["theta"]);
  int32_t const nscales = std::stoi(commandlineArguments["nscales"]);
  int32_t const warps = std::stoi(commandlineArguments["warps"]);
  double const epsilon = std::stod(commandlineArguments["epsilon"]);
  int32_t const innnerIterations = std::stoi(commandlineArguments["innnerIterations"]);
  int32_t const outerIterations = std::stoi(commandlineArguments["outerIterations"]);
  double const scaleStep = std::stod(commandlineArguments["scaleStep"]);
  double const gamma = std::stod(commandlineArguments["gamma"]);
  int32_t const medianFiltering = std::stoi(commandlineArguments["medianFiltering"]);
  bool const useInitialFlow = false;

// tau Time step of the numerical scheme.
// lambda Weight parameter for the data term, attachment parameter. This is the most relevant parameter, which determines the smoothness of the output. The smaller this parameter is, the smoother the solutions we obtain. It depends on the range of motions of the images, so its value should be adapted to each image sequence.
// theta Weight parameter for (u - v)\^2, tightness parameter. It serves as a link between the attachment and the regularization terms. In theory, it should have a small value in order to maintain both parts in correspondence. The method is stable for a large range of values of this parameter.
// nscales Number of scales used to create the pyramid of images.
// warps Number of warpings per scale. Represents the number of times that I1(x+u0) and grad( I1(x+u0) ) are computed per scale. This is a parameter that assures the stability of the method. It also affects the running time, so it is a compromise between speed and accuracy.
// epsilon Stopping criterion threshold used in the numerical scheme, which is a trade-off between precision and running time. A small value will yield more accurate solutions at the expense of a slower convergence.
// iterations Stopping criterion iterations number used in the numerical scheme.

  cv::UMat uImageBefore = cv::imread(imageBeforePath, cv::IMREAD_GRAYSCALE).getUMat(cv::ACCESS_WRITE);
  cv::UMat uImageAfter = cv::imread(imageAfterPath, cv::IMREAD_GRAYSCALE).getUMat(cv::ACCESS_WRITE);
  
  

  cv::UMat flowMat;
  cv::Ptr<cv::optflow::DualTVL1OpticalFlow> dualtlv1 = cv::optflow::createOptFlow_DualTVL1();
  dualtlv1->setTau(tau);
  dualtlv1->setLambda(lambda);
  dualtlv1->setTheta(theta);
  dualtlv1->setScalesNumber(nscales);
  dualtlv1->setWarpingsNumber(warps);
  dualtlv1->setEpsilon(epsilon);
  dualtlv1->setInnerIterations(innnerIterations);
  dualtlv1->setOuterIterations(outerIterations);
  dualtlv1->setScaleStep(scaleStep);
  dualtlv1->setGamma(gamma);
  dualtlv1->setMedianFiltering(medianFiltering);
  dualtlv1->setUseInitialFlow(useInitialFlow); 

  dualtlv1->calc(uImageBefore, uImageAfter, flowMat);
  cv::writeOpticalFlow(outputFloPath, flowMat);

  return 0;
}
