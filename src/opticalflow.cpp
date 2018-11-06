#include "opticalflow.hpp"

#include <opencv2/core/eigen.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


OpticalFlow::OpticalFlow(
    cv::UMat const &a_prevFrameCv, 
    cv::UMat const &a_currentFrameCv, 
    uint32_t a_numBoids, 
    std::vector<Eigen::Vector2i> a_flowOrigins, 
    Eigen::Vector2i a_searchWindowSize, 
    Eigen::Vector2d a_paramPso)
    : m_psoVec()
    , m_prevFrame()
    , m_currentFrame()
    , m_currentFrameCv()
    , m_flowOriginVec(a_flowOrigins)
    , m_currentFlowVec()
    , m_vStream("OpticalFlow")
{

  // std::vector<Eigen::MatrixXi> prevFrameEig = CvBgr2Eigen(a_prevFrameCv);
  // std::vector<Eigen::MatrixXi> currentFrameEig = CvBgr2Eigen(a_currentFrameCv);
  // m_prevFrame = std::make_shared<std::vector<Eigen::MatrixXi>>(prevFrameEig);
  // m_currentFrame = std::make_shared<std::vector<Eigen::MatrixXi>>(currentFrameEig);
  m_prevFrame = std::make_shared<cv::UMat>(a_prevFrameCv);
  m_currentFrame = std::make_shared<cv::UMat>(a_currentFrameCv);
  for (uint32_t i = 0; i < m_flowOriginVec.size(); i++) {
    m_psoVec.emplace_back(m_prevFrame, m_currentFrame, m_flowOriginVec.at(i), a_numBoids, a_paramPso, a_searchWindowSize);
    m_currentFlowVec.emplace_back(Eigen::Vector2i(0,0));
  }  
}
OpticalFlow::~OpticalFlow()
{}

void OpticalFlow::SetNewFrame(cv::UMat const &a_newFrameCv)
{
  // m_currentFrameCv = a_newFrameCv.getMat(0);
  // *m_prevFrame = *m_currentFrame;
  // *m_currentFrame = *std::make_shared<std::vector<Eigen::MatrixXi>>(CvBgr2Eigen(a_newFrameCv)); 
  *m_prevFrame = *std::make_shared<cv::UMat>(a_newFrameCv);
  *m_currentFrame = a_newFrameCv;
  for (uint32_t i = 0; i < m_psoVec.size(); i++) {
    m_psoVec.at(i).Reset();
  }
}

std::vector<Eigen::Vector2i> OpticalFlow::GetOpticalFlow()
{
  #pragma omp parallel for
  for (uint32_t i = 0; i < m_psoVec.size(); i++) {
    for (uint32_t n = 0; n < MAX_ITERATION /*&& m_psoVec.at(i).GetSwarmBestPerformance() > 2*/; n++) {
      m_psoVec.at(i).Step();
      // std::cout << "N " << n << std::endl;
    }
    m_currentFlowVec.at(i) = m_psoVec.at(i).GetFlowVector();
    // std::cout << "Best score: " << m_psoVec.at(i).GetSwarmBestPerformance() << std::endl;
  }
  return m_currentFlowVec;
}

std::vector<Eigen::MatrixXi> OpticalFlow::CvBgr2Eigen(cv::Mat const & a_cvMat)
{
  std::vector<cv::Mat> colorChannels;
  cv::split(a_cvMat, colorChannels);
  std::vector<Eigen::MatrixXi> vectorEigenMat;
  vectorEigenMat.reserve(3);
  for (uint8_t i = 0; i < 3; i++) {
    Eigen::MatrixXi eigMat;
    cv::cv2eigen(colorChannels[i], eigMat);
    vectorEigenMat.push_back(eigMat.transpose().eval());
  }
  return vectorEigenMat;
}

cv::Mat OpticalFlow::Eigen2CvBgr(std::vector<Eigen::MatrixXi> const &a_vectorEigenMat)
{
  std::vector<cv::Mat> colorChannels;

  for (uint8_t i = 0; i < 3; i++) {
    cv::Mat cvMatColor;
    cv::eigen2cv(a_vectorEigenMat.at(i).transpose().eval(), cvMatColor);
    colorChannels.push_back(cvMatColor);
  }
  cv::Mat cvMatBgr;
  cv::merge(colorChannels, cvMatBgr);
  cvMatBgr.convertTo(cvMatBgr, CV_8UC3);
  return cvMatBgr;
}

void OpticalFlow::DrawOpticalFlow()
{
  std::vector<Eigen::Vector2i> endPointVec;
  for (uint32_t i = 0; i < m_psoVec.size(); i++) {
    endPointVec.emplace_back(m_psoVec.at(i).GetSwarmBestPosition());
  }
  cv::Scalar color(255, 0, 0);
  int32_t thickness{1};
  int32_t lineType{8};
  int32_t shift{0};
  double tipLength{0.1};
  // cv::Mat frame = m_currentFrame.clone();
  cv::Mat frame = m_currentFrame->getMat(0);
  for (uint32_t i = 0; i < endPointVec.size(); i++) {
    cv::Point start(m_flowOriginVec[i](0), m_flowOriginVec[i](1));
    cv::Point end(endPointVec[i](1), endPointVec[i](0));
    cv::arrowedLine(frame, start, end, color , thickness, lineType, shift, tipLength);
  }
  m_vStream.SetCurrentFrame(frame);
  m_vStream.Draw();
  cv::waitKey(1);
}