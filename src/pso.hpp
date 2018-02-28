#include <eigen3/Eigen/Dense>
#include <opencv2/core.hpp>
#include <vector>


class Boid
{
private:
  Eigen::Vector2i m_pos;
  Eigen::Vector2i m_vel;
  Eigen::Vector2i m_acc;
public:
  Boid();
  ~Boid();
  void Reset();
  Eigen::Vector2i GetPosition() const;
  Eigen::Vector2i GetVelocity() const;
  Eigen::Vector2i GetAcceleration() const;
  void SetPosition(Eigen::Vector2i const);
  void SetVelocity(Eigen::Vector2i const);
  void SetAcceleration(Eigen::Vector2i const);
  std::string ToString() const;
  void Step();
};


class Pso
{
private:
  uint32_t m_numBoids;
  std::vector<Boid> m_boids;
  Eigen::Vector2i m_bounds;

  void InitializeBoids();
public:
  Pso(uint32_t, Eigen::Vector2i);
  ~Pso();
  void Print();

  cv::Mat GetFrame();
  void Step();
  std::string ToString();
  
};