#include <iostream>
#include <sstream>
#include <limits>

// #include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#include <pso.hpp>

Boid::Boid(ObjectiveFunction a_objFun)
  : m_pos()
  , m_vel()
  , m_acc()
  , m_currentPerformance()
  , m_bestPerformance()
  , m_inertia()
  , m_inertiaDecay()
  , m_objFun(a_objFun)
{
  m_inertiaDecay = 0.995;
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
  m_pos = a_pos;
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
  Eigen::Vector2d vel = (m_inertia + 1) * m_vel + m_acc;
  m_inertia =  m_inertia * m_inertiaDecay;
  SetPosition(pos);
  SetVelocity(vel);

}

Eigen::Vector2i Boid::CheckBoundary(Eigen::Vector2i const a_val)
{
  return m_objFun.CheckBoundary(a_val);
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

Pso::Pso(std::vector<Eigen::MatrixXi> const a_prevImg,
      std::vector<Eigen::MatrixXi> const a_currentImg,
      Eigen::Vector2i a_origin, 
      uint32_t a_numBoids, 
      Eigen::Vector2i a_bounds,
      Eigen::Vector2d a_constants, 
      Eigen::Vector2i a_kernelSize)
  : m_prevImg(a_prevImg)
  , m_currentImg(a_currentImg)
  , m_origin(a_origin)
  , m_numBoids(a_numBoids)
  , m_boids()
  , m_bounds(a_bounds)
  , m_constants(a_constants)
  , m_randGen()
  , m_vMax()
  , m_aMax()
  , m_swarmBestPerformance(std::numeric_limits<double>::max())
  , m_swarmBestPosition()
  , m_kernelSize(a_kernelSize)
{
  m_vMax = 5;
  m_aMax = 20;
  m_swarmBestPosition = m_swarmBestPosition.Zero();
  ObjectiveFunction const objFun(a_prevImg, a_currentImg, a_origin, a_kernelSize);

  InitializeBoids(objFun);
  Scramble();
}

Pso::~Pso()
{}


void Pso::InitializeBoids(ObjectiveFunction const a_objFun)
{
  for (uint32_t i = 0; i < m_numBoids; i++) {
    m_boids.push_back(Boid(a_objFun));
  }
}


void Pso::UpdateAcceleration()
{
  std::uniform_real_distribution<double> distribution(0, 1.0);

  for (std::vector<Boid>::iterator it = m_boids.begin(); it != m_boids.end(); it++) {
    Boid currentBoid = *it;
    if (currentBoid.GetBestPerformance() < m_swarmBestPerformance) {
      m_swarmBestPerformance = currentBoid.GetBestPerformance();
      m_swarmBestPosition = currentBoid.GetBestPosition();
    }
  }

  for (std::vector<Boid>::iterator it = m_boids.begin(); it != m_boids.end(); it++) {
    Boid currentBoid = *it;
    double r = distribution(m_randGen);
    double q = distribution(m_randGen);
    Eigen::Vector2i cognitiveAcc = Eigen::Vector2i::Zero();
    Eigen::Vector2i socialAcc = Eigen::Vector2i::Zero();

    cognitiveAcc = (currentBoid.GetBestPosition() - currentBoid.GetPosition());
    socialAcc = (m_swarmBestPosition - currentBoid.GetPosition()); 
    Eigen::Vector2d acc = m_constants(0) * q * cognitiveAcc.cast<double>() + m_constants(1) * r * socialAcc.cast<double>(); 
    currentBoid.SetAcceleration(acc);
    *it = currentBoid;
  }   
}

void Pso::CheckVelocity()
{
  for (uint32_t i = 0; i < m_numBoids; i++) {
    if (m_boids.at(i).GetVelocity().norm() > m_vMax) {
      Eigen::Vector2d vel = m_boids.at(i).GetVelocity();
      vel.normalize();
      vel = vel * m_vMax;
      m_boids.at(i).SetVelocity(vel);
    }
  }
}

cv::Mat Pso::GetFrame() const
{
  cv::Mat frame(m_bounds(0), m_bounds(1), CV_8UC3, cv::Scalar(0, 0, 0));
  cv::Vec3b color(0,0,255);
  for (uint32_t i = 0; i < m_numBoids; i++) {
    Eigen::Vector2i pos = m_boids.at(i).GetPosition();
    cv::circle(frame, cv::Point(pos(1),pos(0)), 3, color, -1, 8); 
  }
  return frame;
}

cv::Mat Pso::GetFrame(cv::Mat a_mat) const
{
  cv::Vec3b color(0,0,255);
  for (uint32_t i = 0; i < m_numBoids; i++) {
    Eigen::Vector2i pos = m_boids.at(i).GetPosition();
    cv::circle(a_mat, cv::Point(pos(1),pos(0)), 3, color, -1, 8); 
  }
  return a_mat;
}

void Pso::Step()
{
  CrazyCheck();
  UpdateAcceleration();
  for (uint32_t i = 0; i < m_numBoids; i++) {
    m_boids.at(i).Step();
  }
  CheckVelocity();
}

std::string Pso::ToString()
{
  std::stringstream ss;
  for (uint32_t i = 0; i < m_numBoids; i++) {
    ss << i << ": ";
    ss << m_boids.at(i).ToString();
    ss << std::endl;
  }
  ss << "Best: " << m_swarmBestPerformance << "  at " << m_swarmBestPosition; 
  ss << std::endl;
  return ss.str();
}

Boid Pso::RandomizeBoidUniform(Boid a_boid)
{
  std::uniform_real_distribution<double> distributionPosX(0.0, static_cast<double>(m_bounds(0)));
  std::uniform_real_distribution<double> distributionPosY(0.0, static_cast<double>(m_bounds(1)));
  std::uniform_real_distribution<double> distribution(0, 1.0);
  Eigen::Vector2d pos;
  Eigen::Vector2d vel;
  Eigen::Vector2d acc;
  vel.setRandom();
  acc.setRandom();
  vel.normalize();
  acc.normalize();
  vel = vel * distribution(m_randGen) * m_vMax;
  acc = acc * distribution(m_randGen) * 3.0 * m_aMax;
  pos << distributionPosX(m_randGen), distributionPosY(m_randGen);
  a_boid.SetPosition(pos);
  a_boid.SetVelocity(vel);
  a_boid.SetAcceleration(acc);
  return a_boid;
}

void Pso::Scramble()
{
  for (std::vector<Boid>::iterator it = m_boids.begin(); it != m_boids.end(); it++) {
    Boid currentBoid = *it;
    *it = RandomizeBoidUniform(currentBoid);
  }
}

void Pso::CrazyCheck()
{
  std::uniform_real_distribution<double> distribution(0, 1.0);
  for (std::vector<Boid>::iterator it = m_boids.begin(); it != m_boids.end(); it++) {
    Boid currentBoid = *it;
    if (distribution(m_randGen) < 0.01) {
      Eigen::Vector2d vel;
      vel.normalize();
      vel = vel * distribution(m_randGen) * m_vMax;
      currentBoid.SetVelocity(vel);
    }
    *it = currentBoid;
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