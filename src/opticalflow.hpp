
#include <opencv2/core.hpp>
#include <eigen3/Eigen/Dense>
#include <memory>

#include "pso.hpp"
#include "videostream.hpp"

class OpticalFlow
{
  private:
    std::vector<Pso> m_psoVec;
    // std::shared_ptr<std::vector<Eigen::MatrixXi>> m_prevFrame;
    // std::shared_ptr<std::vector<Eigen::MatrixXi>> m_currentFrame;
    std::shared_ptr<cv::UMat> m_prevFrame;
    std::shared_ptr<cv::UMat> m_currentFrame;
    cv::Mat m_currentFrameCv;
    std::vector<Eigen::Vector2i> m_flowOriginVec;
    std::vector<Eigen::Vector2i> m_currentFlowVec;
    VideoStream m_vStream;
    uint32_t const MAX_ITERATION = 500;

    std::vector<Eigen::MatrixXi> CvBgr2Eigen(cv::Mat const &);
    cv::Mat Eigen2CvBgr(std::vector<Eigen::MatrixXi> const &);



  public:
    OpticalFlow(cv::UMat const &, cv::UMat const &, uint32_t, std::vector<Eigen::Vector2i>, Eigen::Vector2i, Eigen::Vector2d);
    ~OpticalFlow();
    OpticalFlow(const OpticalFlow &) = delete;
    OpticalFlow(OpticalFlow &&) = delete;
    OpticalFlow &operator=(const OpticalFlow &) = delete;
    OpticalFlow &operator=(OpticalFlow &&) = delete;

    void SetNewFrame(cv::UMat const &);
    std::vector<Eigen::Vector2i> GetOpticalFlow();
    void DrawOpticalFlow();
    
};