#include <iostream>
#include <sstream>
#include <limits>

#include <opencv2/imgproc.hpp>

#include <pso.hpp>

Boid::Boid(ObjectiveFunction const &a_objFun)
  : m_pos()
  , m_vel()
  , m_acc()
  , m_currentPerformance()
  , m_bestPerformance()
  , m_inertia()
  , m_inertiaDecay()
  , m_objFun(a_objFun)
{
  m_inertiaDecay = 0.99;
  Reset();
}

Boid::~Boid()
{}

void Boid::Reset()
{
  SetPosition(Eigen::Vector2i(0,0));
  SetVelocity(Eigen::Vector2d(0,0));
  SetAcceleration(Eigen::Vector2d(0,0));
  m_bestPerformance.first = std::numeric_limits<double>::max();
  m_bestPerformance.second.Zero();
  m_inertia = 20;
}

Eigen::Vector2i Boid::GetPosition() const
{
  return m_pos;
}

Eigen::Vector2d Boid::GetVelocity() const
{
  return m_vel;
}

Eigen::Vector2d Boid::GetAcceleration() const
{
  return m_acc;
}

void Boid::SetPosition(Eigen::Vector2i const a_pos)
{
  m_pos = m_objFun.CheckBoundary(a_pos);
  m_currentPerformance = m_objFun.GetScore(m_pos);
  if (m_currentPerformance < m_bestPerformance.first) {
    m_bestPerformance = std::pair<double, Eigen::Vector2i>(m_currentPerformance, a_pos);
  }
}

void Boid::SetPosition(Eigen::Vector2d const a_pos)
{
  Eigen::Vector2i pos =  a_pos.array().round().cast<int32_t>();
  SetPosition(pos);
}

void Boid::SetVelocity(Eigen::Vector2d const a_val)
{
  m_vel = a_val;
}

void Boid::SetAcceleration(Eigen::Vector2d const a_val)
{
  m_acc = a_val;
}

std::string Boid::ToString()
{
  std::stringstream ss;
  Eigen::IOFormat CleanFmt(0, 0, " ", " ", "", "");
  ss << "Pos: " << m_pos.format(CleanFmt) << " Vel: " << m_vel.format(CleanFmt) << " Acc: " << m_acc.format(CleanFmt);
  ss << "\n";
  ss << "Inertia: " << m_inertia;
  ss << "\n";
  ss << "Current performance: " << m_currentPerformance;
  ss << "\n";
  ss << "Best performance: " << m_bestPerformance.first << ", at " << m_bestPerformance.second.format(CleanFmt);
  return ss.str();
}

void Boid::Step()
{
  Eigen::Vector2i pos = (m_pos.cast<double>() + m_vel).array().round().cast<int32_t>();
  Eigen::Vector2d vel = (m_inertia + 0.7) * m_vel + m_acc;
  m_inertia =  m_inertia * m_inertiaDecay;
  SetPosition(pos);
  SetVelocity(vel);

}


double Boid::GetPerformance() const
{
  return m_currentPerformance;
}

double Boid::GetBestPerformance() const
{
  return m_bestPerformance.first;
}

Eigen::Vector2i Boid::GetBestPosition() const
{
  return m_bestPerformance.second;
}


/*
  PSO
*/

Pso::Pso(
      // std::shared_ptr<std::vector<Eigen::MatrixXi>> a_prevImg,
      // std::shared_ptr<std::vector<Eigen::MatrixXi>> a_currentImg,
      std::shared_ptr<cv::UMat> const &a_prevImg,
      std::shared_ptr<cv::UMat> const &a_currentImg,
      Eigen::Vector2i a_origin, 
      uint32_t a_numBoids, 
      Eigen::Vector2d a_constants, 
      Eigen::Vector2i a_kernelSize)
  : m_prevImg(a_prevImg)
  , m_currentImg(a_currentImg)
  , m_origin(a_origin)
  , m_numBoids(a_numBoids)
  , m_boids()
  , m_constants(a_constants)
  , m_randGen()
  , m_vMax()
  , m_aMax()
  , m_swarmBestPerformance()
  , m_swarmBestPosition()
  , m_kernelSize(a_kernelSize)
{
  std::random_device r;
  m_randGen = std::default_random_engine(r());
  m_vMax = 3;
  m_aMax = 10;

  ObjectiveFunction objFun(a_prevImg, a_currentImg, a_origin, a_kernelSize);
  for (uint32_t i = 0; i < m_numBoids; i++) {
    m_boids.emplace_back(objFun);
  }
  Reset();
}

Pso::~Pso()
{}



void Pso::UpdateAcceleration()
{
  std::uniform_real_distribution<double> distribution(0, 1.0);
  for (uint32_t i = 0; i < m_numBoids; i++) {
    if (m_boids[i].GetBestPerformance() < m_swarmBestPerformance) {
      m_swarmBestPerformance = m_boids[i].GetBestPerformance();
      m_swarmBestPosition = m_boids[i].GetBestPosition();
    }
  }
  for (uint32_t i = 0; i < m_numBoids; i++) {
    double r = distribution(m_randGen);
    double q = distribution(m_randGen);
    Eigen::Vector2i cognitiveAcc = Eigen::Vector2i::Zero();
    Eigen::Vector2i socialAcc = Eigen::Vector2i::Zero();

    cognitiveAcc = (m_boids[i].GetBestPosition() - m_boids[i].GetPosition());
    socialAcc = (m_swarmBestPosition - m_boids[i].GetPosition()); 
    Eigen::Vector2d acc = m_constants(0) * q * cognitiveAcc.cast<double>() + m_constants(1) * r * socialAcc.cast<double>(); 
    m_boids[i].SetAcceleration(acc);
  }   
}

void Pso::CheckVelocity()
{
  for (uint32_t i = 0; i < m_numBoids; i++) {
    if (m_boids[i].GetVelocity().norm() > m_vMax) {
      Eigen::Vector2d vel = m_boids[i].GetVelocity();
      vel.normalize();
      vel = vel * m_vMax;
      m_boids[i].SetVelocity(vel);
    }
  }
}

cv::Mat Pso::GetFrame(cv::Mat a_mat) const
{
  cv::Vec3b color(0,0,255);
  for (uint32_t i = 0; i < m_numBoids; i++) {
    Eigen::Vector2i pos = m_boids[i].GetPosition();
    cv::circle(a_mat, cv::Point(pos(1),pos(0)), 3, color, -1, 8); 
  }
  return a_mat;
}

void Pso::Step()
{
  CrazyCheck();
  UpdateAcceleration();
  // #pragma omp parallel for
  for (uint32_t i = 0; i < m_numBoids; i++) {
    m_boids[i].Step();
  }
  CheckVelocity();
}

std::string Pso::ToString()
{
  std::stringstream ss;
  for (uint32_t i = 0; i < m_numBoids; i++) {
    ss << i << ": ";
    ss << m_boids[i].ToString();
    ss << std::endl;
  }
  ss << "Best: " << m_swarmBestPerformance << "  at " << m_swarmBestPosition; 
  ss << std::endl;
  return ss.str();
}


void Pso::ScrambleUniform()
{
  std::uniform_real_distribution<> distributionPosX(0.0, static_cast<double>((*m_currentImg).rows));
  std::uniform_real_distribution<> distributionPosY(0.0, static_cast<double>((*m_currentImg).cols));
  std::uniform_real_distribution<> distribution(0, 1.0);
  Eigen::Vector2d pos;
  Eigen::Vector2d vel;
  Eigen::Vector2d acc;
  for (uint32_t i = 0; i < m_numBoids; i++) {
    vel.setRandom();
    acc.setRandom();
    vel.normalize();
    acc.normalize();
    vel = vel * distribution(m_randGen) * m_vMax;
    acc = acc * distribution(m_randGen) * m_aMax;
    pos << distributionPosX(m_randGen), distributionPosY(m_randGen);
    m_boids[i].SetPosition(pos);
    m_boids[i].SetVelocity(vel);
    m_boids[i].SetAcceleration(acc);
  }
}

void Pso::ScrambleNormal()
{
  std::normal_distribution<> normaldist{0,10};
  std::uniform_real_distribution<> distribution(0, 1.0);
  Eigen::Vector2d pos;
  Eigen::Vector2d vel;
  Eigen::Vector2d acc;
  for (uint32_t i = 0; i < m_numBoids; i++) {
    pos.setRandom();
    vel.setRandom();
    acc.setRandom();
    pos.normalize();
    vel.normalize();
    acc.normalize();
    vel = vel * distribution(m_randGen) * m_vMax;
    acc = acc * distribution(m_randGen) * m_aMax;
    pos = pos * normaldist(m_randGen);
    pos = m_origin.cast<double>() + pos;
    m_boids[i].SetPosition(pos);
    m_boids[i].SetVelocity(vel);
    m_boids[i].SetAcceleration(acc);
  }
}

void Pso::CrazyCheck()
{
  std::uniform_real_distribution<> distribution(0, 1.0);
  for (uint32_t i = 0; i < m_numBoids; i++) {
    if (distribution(m_randGen) < 0.01) {
      Eigen::Vector2d vel;
      vel.normalize();
      vel = vel * distribution(m_randGen) * m_vMax;
      m_boids[i].SetVelocity(vel);
    }
  }
}

double Pso::GetSwarmBestPerformance() const
{
  return m_swarmBestPerformance;
}

Eigen::Vector2i Pso::GetSwarmBestPosition() const
{
  return m_swarmBestPosition;
}
Eigen::Vector2i Pso::GetFlowVector() const
{
  return m_swarmBestPosition - m_origin;
}

void Pso::Reset()
{
  m_swarmBestPerformance = std::numeric_limits<double>::max();
  m_swarmBestPosition = m_swarmBestPosition.Zero();
  for (uint32_t i = 0; i < m_numBoids; i++) {
    m_boids[i].Reset();
  }
  ScrambleNormal();
}