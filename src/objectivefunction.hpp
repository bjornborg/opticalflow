#include <vector>
#include <eigen3/Eigen/Dense>
#include <memory>


class ObjectiveFunction
{
private:
  std::shared_ptr<std::vector<Eigen::MatrixXi>> m_prevImg;
  std::shared_ptr<std::vector<Eigen::MatrixXi>> m_currentImg;
  Eigen::Vector2i m_origin;
  Eigen::Vector2i m_kernelSize;
  Eigen::Vector2i m_halfKernelSize;
  Eigen::Vector2i m_bounds;
public:
  ObjectiveFunction(std::shared_ptr<std::vector<Eigen::MatrixXi>> ,
      std::shared_ptr<std::vector<Eigen::MatrixXi>> ,
      Eigen::Vector2i const, 
      Eigen::Vector2i const);
  ~ObjectiveFunction();
  double GetScore(Eigen::Vector2i const);
  Eigen::Vector2i CheckBoundary(Eigen::Vector2i const);
};