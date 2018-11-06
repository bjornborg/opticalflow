#include <objectivefunction.hpp>
#include <cmath>



ObjectiveFunction::ObjectiveFunction(
        // std::shared_ptr<std::vector<Eigen::MatrixXi>> a_prevImg, 
        // std::shared_ptr<std::vector<Eigen::MatrixXi>> a_currentImg, 
        std::shared_ptr<cv::UMat> const &a_prevImg,
        std::shared_ptr<cv::UMat> const &a_currentImg,
        Eigen::Vector2i const a_origin, 
        Eigen::Vector2i const a_kernelSize)
    : m_prevImg(a_prevImg)
    , m_currentImg(a_currentImg)
    , m_origin(a_origin)
    , m_kernelSize(a_kernelSize)
    , m_halfKernelSize(a_kernelSize / 2)
    , m_bounds()

{
  // m_halfKernelSize = a_kernelSize / 2;
  // m_bounds << a_prevImg->at(0).rows(), a_prevImg->at(0).cols();
  m_bounds << (*a_prevImg).rows, (*a_prevImg).cols;
}
ObjectiveFunction::~ObjectiveFunction()
{}

double ObjectiveFunction::GetScore(Eigen::Vector2i const &a_currentPoints)
{
  (void) a_currentPoints;
  double val = 0;
  cv::UMat dst;
  // cv::Range kernelrow(m_origin(0)-m_halfKernelSize(0), m_origin(0)-m_halfKernelSize(0)+m_kernelSize(0));
  // cv::Range kernelcol(m_origin(1)-m_halfKernelSize(1), m_origin(1)-m_halfKernelSize(1)+m_kernelSize(1));
  cv::Rect rec0(m_origin(0)-m_halfKernelSize(0), m_origin(1)-m_halfKernelSize(1), m_kernelSize(0), m_kernelSize(1));
  cv::Rect rec1(a_currentPoints(1)-m_halfKernelSize(0), a_currentPoints(0)-m_halfKernelSize(1), m_kernelSize(0), m_kernelSize(1));
  
  cv::UMat roi0 = cv::UMat(*m_prevImg, rec0);
  cv::UMat roi1 = cv::UMat(*m_currentImg, rec1);
  cv::absdiff(roi0, roi1, dst);
  val = cv::sum(cv::sum(dst)).val[0];
  // cv::MatOp::roi(test, kernelrow, kernelcol, temp1);




  // for (uint8_t i = 0; i < m_prevImg->size(); i++) {
  //   val += (m_prevImg->at(i).block(m_origin(0)-m_halfKernelSize(0), m_origin(1)-m_halfKernelSize(1), m_kernelSize(0), m_kernelSize(1)) - m_currentImg->at(i).block(a_currentPoints(0)-m_halfKernelSize(0), a_currentPoints(1)-m_halfKernelSize(1), m_kernelSize(0), m_kernelSize(1))).lpNorm<1>();
  // }
  // val = val/m_prevImg->size();
  // val = val/(m_kernelSize(0)*m_kernelSize(1));
  // val = val + (a_currentPoints - m_origin).norm()/5;
  return val;
}

Eigen::Vector2i ObjectiveFunction::CheckBoundary(Eigen::Vector2i a_pos)
{
  for (uint32_t j = 0; j < a_pos.size(); j++) {
    if (a_pos(j) < m_halfKernelSize(j)) {
      a_pos(j) = m_halfKernelSize(j);
    }
    if (a_pos(j) >= m_bounds(j) - m_halfKernelSize(j)) {
      a_pos(j) = m_bounds(j) - m_halfKernelSize(j) - 1;
    }
  }
  return a_pos;
}
