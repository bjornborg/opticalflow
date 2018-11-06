#include <vector>
#include <eigen3/Eigen/Dense>
#include <memory>
#include <opencv2/core.hpp>


class ObjectiveFunction
{
private:
  // std::shared_ptr<std::vector<Eigen::MatrixXi>> m_prevImg;
  // std::shared_ptr<std::vector<Eigen::MatrixXi>> m_currentImg;
  std::shared_ptr<cv::UMat> m_prevImg;
  std::shared_ptr<cv::UMat> m_currentImg;
  Eigen::Vector2i const m_origin;
  Eigen::Vector2i const m_kernelSize;
  Eigen::Vector2i const m_halfKernelSize;
  Eigen::Vector2i m_bounds;
public:
  ObjectiveFunction(
      // std::shared_ptr<std::vector<Eigen::MatrixXi>> ,
      // std::shared_ptr<std::vector<Eigen::MatrixXi>> ,
      std::shared_ptr<cv::UMat> const &,
      std::shared_ptr<cv::UMat> const &,
      Eigen::Vector2i const, 
      Eigen::Vector2i const);
  ~ObjectiveFunction();
  double GetScore(Eigen::Vector2i const &);
  Eigen::Vector2i CheckBoundary(Eigen::Vector2i);
};