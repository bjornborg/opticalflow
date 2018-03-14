#include <vector>
#include <eigen3/Eigen/Dense>
#include <opencv2/core.hpp>


class ObjectiveFunction
{
private:
  std::vector<Eigen::MatrixXi> m_prevImg;
  std::vector<Eigen::MatrixXi> m_currentImg;
  Eigen::Vector2i m_origin;
  Eigen::Vector2i m_kernelSize;
  Eigen::Vector2i m_halfKernelSize;
  Eigen::Vector2i m_bounds;
public:
  ObjectiveFunction(std::vector<Eigen::MatrixXi> const,
      std::vector<Eigen::MatrixXi> const,
      Eigen::Vector2i const, 
      Eigen::Vector2i const);
  ~ObjectiveFunction();
  double GetScore(Eigen::Vector2i const);
  Eigen::Vector2i CheckBoundary(Eigen::Vector2i const);

  static std::vector<Eigen::MatrixXi> CvBgr2Eigen(cv::Mat);
  static cv::Mat Eigen2CvBgr(std::vector<Eigen::MatrixXi>);

};