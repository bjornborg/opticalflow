#include <random>
#include <vector>
#include <utility> 
#include <opencv2/core.hpp>
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
public:
  Boid(ObjectiveFunction const &);
  ~Boid();
  Boid(const Boid &) = default;
  Boid(Boid &&) = default;
  Boid &operator=(const Boid &) = default;
  Boid &operator=(Boid &&) = default;
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
  // std::shared_ptr<std::vector<Eigen::MatrixXi>> m_prevImg;
  // std::shared_ptr<std::vector<Eigen::MatrixXi>> m_currentImg;
  std::shared_ptr<cv::UMat> m_prevImg;
  std::shared_ptr<cv::UMat> m_currentImg;
  Eigen::Vector2i m_origin;
  uint32_t m_numBoids;
  std::vector<Boid> m_boids;
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

public:
  Pso(
      // std::shared_ptr<std::vector<Eigen::MatrixXi>> ,
      // std::shared_ptr<std::vector<Eigen::MatrixXi>> ,
      std::shared_ptr<cv::UMat> const &,
      std::shared_ptr<cv::UMat> const &,
      Eigen::Vector2i,
      uint32_t, 
      Eigen::Vector2d, 
      Eigen::Vector2i);
  ~Pso();
  Pso(const Pso &) = default;
  Pso(Pso &&) = default;
  Pso &operator=(const Pso &) = default;
  Pso &operator=(Pso &&) = default;
  void Print();

  cv::Mat GetFrame(cv::Mat) const;
  void Step();
  std::string ToString();
  void ScrambleUniform();
  void ScrambleNormal();
  double GetSwarmBestPerformance() const;
  Eigen::Vector2i GetSwarmBestPosition() const;
  Eigen::Vector2i GetFlowVector() const;
  void Reset();
};