#include <videostream.hpp>

VideoStream::VideoStream(std::string a_streamPath)
  : m_capstream()
  , m_currentFrame()
  , m_window()
  , m_frameCounter(0)
{
  // m_capstream = cv::VideoCapture(a_streamPath);
  m_window = a_streamPath;
  // cv::namedWindow(m_window);
  // cv::namedWindow(m_window + "-previous");
}
VideoStream::VideoStream()
  : m_capstream()
  , m_currentFrame()
  , m_window()
  , m_frameCounter(0)
{}

VideoStream::~VideoStream()
{
  m_capstream.release();
  m_currentFrame.release();
}

bool VideoStream::NextFrame()
{
  m_frameCounter++;
  return m_capstream.read(m_currentFrame);
}

void VideoStream::Draw()
{
  if (!m_currentFrame.empty()) {
    cv::imshow(m_window, m_currentFrame);
  }
  cv::waitKey(1);
}

uint32_t VideoStream::GetFrameCounter()
{
  return m_frameCounter;
}

void VideoStream::SetCurrentFrame(cv::Mat a_mat)
{
  m_currentFrame.release();
  m_currentFrame = a_mat;
}
