#include <eigen3/Eigen/Dense>
#include <opencv2/core.hpp>
#include <random>
#include <vector>
#include <utility> 

#include <objectivefunction.hpp>

class Boid
{
private:
  Eigen::Vector2i m_pos;
  Eigen::Vector2d m_vel;
  Eigen::Vector2d m_acc;
  double m_currentPerformance;
  std::pair<double, Eigen::Vector2i> m_bestPerformance;
  double m_inertia;
  double m_inertiaDecay;
  ObjectiveFunction m_objFun;

  void UpdatePerformance();
  Eigen::Vector2i CheckBoundary(Eigen::Vector2i const);
public:
  Boid(ObjectiveFunction const);
  ~Boid();
  void Reset();
  Eigen::Vector2i GetPosition() const;
  Eigen::Vector2d GetVelocity() const;
  Eigen::Vector2d GetAcceleration() const;
  void SetPosition(Eigen::Vector2i const);
  void SetPosition(Eigen::Vector2d const);
  void SetVelocity(Eigen::Vector2d const);
  void SetAcceleration(Eigen::Vector2d const);
  std::string ToString();
  void Step();
  void SetPerformance(double);
  double GetPerformance() const;
  double GetBestPerformance() const;
  Eigen::Vector2i GetBestPosition() const;
};


class Pso
{
private:
  std::vector<Eigen::MatrixXi> m_prevImg;
  std::vector<Eigen::MatrixXi> m_currentImg;
  Eigen::Vector2i m_origin;
  uint32_t m_numBoids;
  std::vector<Boid> m_boids;
  Eigen::Vector2i m_bounds;
  Eigen::Vector2d m_constants;
  std::default_random_engine m_randGen;
  double m_vMax;
  double m_aMax;
  double m_swarmBestPerformance;
  Eigen::Vector2i m_swarmBestPosition;
  Eigen::Vector2i m_kernelSize;


  void InitializeBoids(ObjectiveFunction const);
  void UpdateAcceleration();
  void CheckVelocity();
  void CrazyCheck();
  Boid RandomizeBoidUniform(Boid);

public:
  Pso(std::vector<Eigen::MatrixXi>  const,
      std::vector<Eigen::MatrixXi>  const,
      Eigen::Vector2i,
      uint32_t, 
      Eigen::Vector2i, 
      Eigen::Vector2d, 
      Eigen::Vector2i);
  ~Pso();
  void Print();

  cv::Mat GetFrame() const;
  cv::Mat GetFrame(cv::Mat) const;
  void Step();
  std::string ToString();
  void Scramble();
  double GetSwarmBestPerformance() const;
  Eigen::Vector2i GetSwarmBestPosition() const;
};