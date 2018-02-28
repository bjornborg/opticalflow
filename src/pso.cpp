#include <iostream>
#include <pso.hpp>
#include <sstream>

Boid::Boid()
  : m_pos()
  , m_vel()
  , m_acc()
{
  Reset();  
}

Boid::~Boid()
{}

void Boid::Reset()
{
  m_pos << 0,0;
  m_vel << 0,0;
  m_acc << 0,0;
}

Eigen::Vector2i Boid::GetPosition() const
{
  return m_pos;
}

Eigen::Vector2i Boid::GetVelocity() const
{
  return m_vel;
}

Eigen::Vector2i Boid::GetAcceleration() const
{
  return m_acc;
}

void Boid::SetPosition(Eigen::Vector2i const a_val)
{
  m_pos = a_val;
}

void Boid::SetVelocity(Eigen::Vector2i const a_val)
{
  m_vel = a_val;
}

void Boid::SetAcceleration(Eigen::Vector2i const a_val)
{
  m_acc = a_val;
}

std::string Boid::ToString() const
{
  std::stringstream ss;
  Eigen::IOFormat CleanFmt(0, 0, " ", " ", "", "");
  ss << "Pos: " << m_pos.format(CleanFmt) << " Vel: " << m_vel.format(CleanFmt) << " Acc: " << m_acc.format(CleanFmt);
  return ss.str();
}

void Boid::Step()
{
  m_pos = m_pos + m_vel;
  m_vel = m_vel + m_acc;
}



Pso::Pso(uint32_t a_numBoids, Eigen::Vector2i a_bounds)
  : m_numBoids()
  , m_boids()
  , m_bounds()
{
  m_numBoids = a_numBoids;
  m_bounds = a_bounds;
  InitializeBoids();
}

Pso::~Pso()
{}


void Pso::InitializeBoids()
{
  for (uint32_t i = 0; i < m_numBoids; i++) {
    m_boids.push_back(Boid());
  }
}


cv::Mat Pso::GetFrame()
{
  cv::Mat frame(m_bounds(0), m_bounds(1), CV_8UC3, cv::Scalar(0, 0, 0));
  for (uint32_t i = 0; i < m_numBoids; i++) {
    Eigen::Vector2i pos = m_boids.at(i).GetPosition();
    frame.at<uchar>(pos(0),pos(1),0) = 250;
    std::cout << pos << std::endl;
  }
  return frame;
}

void Pso::Step()
{
  for (uint32_t i = 0; i < m_numBoids; i++) {
    m_boids.at(i).Step();
    if((m_boids.at(i).GetPosition().array() > m_bounds.array()).any() 
        || (m_boids.at(i).GetPosition().array() < 0).any()) {
      m_boids.at(i).Reset();
    }
  }
}

std::string Pso::ToString()
{
  std::stringstream ss;
  for (uint32_t i = 0; i < m_numBoids; i++) {
    ss << i << ": ";
    ss << m_boids.at(i).ToString();
    ss << std::endl;
  }
  return ss.str();
}