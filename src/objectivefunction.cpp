#include <objectivefunction.hpp>
#include <opencv2/core/eigen.hpp>
#include <cmath>
#include <iostream>


#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp> 

ObjectiveFunction::ObjectiveFunction(std::vector<Eigen::MatrixXi> const a_prevImg, 
        std::vector<Eigen::MatrixXi> const a_currentImg, 
        Eigen::Vector2i const a_origin, 
        Eigen::Vector2i const a_kernelSize)
    : m_prevImg(a_prevImg)
    , m_currentImg(a_currentImg)
    , m_origin(a_origin)
    , m_kernelSize(a_kernelSize)
    , m_halfKernelSize()
    , m_bounds()

{
  m_halfKernelSize = a_kernelSize / 2;
  m_bounds << a_prevImg.at(0).rows(), a_prevImg.at(0).cols();
}
ObjectiveFunction::~ObjectiveFunction()
{}

double ObjectiveFunction::GetScore(Eigen::Vector2i const a_currentPoints)
{
  double val = 0;
  for (uint8_t i = 0; i < m_prevImg.size(); i++) {
    Eigen::MatrixXi prevFrame = m_prevImg.at(i);
    Eigen::MatrixXi currentFrame = m_currentImg.at(i);
    Eigen::MatrixXi subPrevframe = prevFrame.block(m_origin(0)-m_halfKernelSize(0), m_origin(1)-m_halfKernelSize(1), m_kernelSize(0), m_kernelSize(1));
    Eigen::MatrixXi subCurrentframe = currentFrame.block(a_currentPoints(0)-m_halfKernelSize(0), a_currentPoints(1)-m_halfKernelSize(1), m_kernelSize(0), m_kernelSize(1));
    val += (subPrevframe - subCurrentframe).array().abs().sum();
  }
  return val;
}

Eigen::Vector2i ObjectiveFunction::CheckBoundary(Eigen::Vector2i a_pos)
{
  for (uint32_t j = 0; j < a_pos.size(); j++) {
    if (a_pos(j) < m_halfKernelSize(j)) {
      a_pos(j) = m_halfKernelSize(j);
    }
    if (a_pos(j) > m_bounds(j)-m_halfKernelSize(j)) {
      a_pos(j) = m_bounds(j)-m_halfKernelSize(j);
    }
  }
  return a_pos;
}


std::vector<Eigen::MatrixXi> ObjectiveFunction::CvBgr2Eigen(cv::Mat a_cvMat)
{
  std::vector<cv::Mat> colorChannels;
  cv::split(a_cvMat, colorChannels);
  std::vector<Eigen::MatrixXi> vectorEigenMat;
  for (uint8_t i = 0; i < 3; i++) {
    Eigen::MatrixXi eigMat;
    cv::cv2eigen(colorChannels[i], eigMat);
    vectorEigenMat.push_back(eigMat);
  }
  return vectorEigenMat;
}

cv::Mat ObjectiveFunction::Eigen2CvBgr(
    std::vector<Eigen::MatrixXi> a_vectorEigenMat)
{
  std::vector<cv::Mat> colorChannels;

  for (uint8_t i = 0; i < 3; i++) {
    cv::Mat cvMatColor;
    cv::eigen2cv(a_vectorEigenMat[i], cvMatColor);
    colorChannels.push_back(cvMatColor);
  }
  cv::Mat cvMatBgr;
  cv::merge(colorChannels, cvMatBgr);
  cvMatBgr.convertTo(cvMatBgr, CV_8UC3);
  return cvMatBgr;
}
