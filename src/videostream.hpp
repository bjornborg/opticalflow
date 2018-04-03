#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp> 

class VideoStream
{
private:
  cv::VideoCapture m_capstream;
  cv::Mat m_currentFrame;
  std::string m_window;
  uint32_t m_frameCounter; 
public:
  VideoStream(std::string);
  VideoStream();
  ~VideoStream();
  bool NextFrame();
  void Draw();
  uint32_t GetFrameCounter();
  void SetCurrentFrame(cv::Mat);
  
};