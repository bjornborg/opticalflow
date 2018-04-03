#include <objectivefunction.hpp>
#include <cmath>



ObjectiveFunction::ObjectiveFunction(
        std::shared_ptr<std::vector<Eigen::MatrixXi>> a_prevImg, 
        std::shared_ptr<std::vector<Eigen::MatrixXi>> a_currentImg, 
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
  m_bounds << a_prevImg->at(0).rows(), a_prevImg->at(0).cols();
}
ObjectiveFunction::~ObjectiveFunction()
{}

double ObjectiveFunction::GetScore(Eigen::Vector2i const a_currentPoints)
{
  double val = 0;
  for (uint8_t i = 0; i < m_prevImg->size(); i++) {
    val += (m_prevImg->at(i).block(m_origin(0)-m_halfKernelSize(0), m_origin(1)-m_halfKernelSize(1), m_kernelSize(0), m_kernelSize(1)) - m_currentImg->at(i).block(a_currentPoints(0)-m_halfKernelSize(0), a_currentPoints(1)-m_halfKernelSize(1), m_kernelSize(0), m_kernelSize(1))).array().abs().sum();
  }
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
