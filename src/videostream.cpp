#include <videostream.hpp>

VideoStream::VideoStream(std::string a_streamPath)
  : m_capstream()
  , m_currentFrame()
  , m_previousFrame()
  , m_window()
  , m_frameCounter(0)
{
  m_capstream = cv::VideoCapture(a_streamPath);
  m_window = a_streamPath;
  cv::namedWindow(m_window);
  cv::namedWindow(m_window + "-previous");
}
VideoStream::VideoStream()
  : m_capstream()
  , m_currentFrame()
  , m_previousFrame()
  , m_window()
  , m_frameCounter(0)
{
  cv::namedWindow("Debug");
  m_window = "Debug";


}

VideoStream::~VideoStream()
{
  m_capstream.release();
  m_currentFrame.release();
  m_previousFrame.release();
}

bool VideoStream::NextFrame()
{
  m_previousFrame = m_currentFrame.clone();
  m_frameCounter++;
  return m_capstream.read(m_currentFrame);
}

void VideoStream::Draw()
{
  if (!m_currentFrame.empty()) {
    cv::imshow(m_window, m_currentFrame);
  }
  if (!m_previousFrame.empty()) {
    cv::imshow(m_window + "-previous", m_previousFrame);
  }
  cv::waitKey(1);
}

uint32_t VideoStream::GetFrameCounter()
{
  return m_frameCounter;
}

cv::Mat VideoStream::GetPreviousFrame()
{
  return m_previousFrame;
}

cv::Mat VideoStream::GetCurrentFrame()
{
  return m_currentFrame;
}

void VideoStream::SetCurrentFrame(cv::Mat a_mat)
{
  m_currentFrame.release();
  m_currentFrame = a_mat;
}
