#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <pthread.h>
#include <daq/Callbacks.h>

namespace bob { namespace daq {

/**
 * Camera is an abstract class which captures frames
 */
class Camera : public Stoppable {
public:

  /**
   * Pixel format
   */
  enum PixelFormat {
    OTHER,
    YUYV,
    MJPEG,
    RGB24
  };

  /**
   *  Callback provided by Camera
   */
  class CameraCallback {
  public:
    
    /**
     * Image received by the camera. The implementation should be as short as
     * possible since the capture thread is blocked during this call
     *
     * @param image       image buffer
     * @param pixelFormat pixel format of the image
     * @param width       image width
     * @param height      image height
     * @param stride      image stride
     * @param size        total image size (usually height*stride)
     * @param frameNb     frame number
     * @param timestamp   frame timestamp (in seconds)
     */
    virtual void imageReceived(unsigned char* image, PixelFormat pixelFormat, int width, int height, int stride, int size, int frameNb, double timestamp) = 0;
  };

  /**
   * Frame size
   */
  struct FrameSize {
    FrameSize(int width, int height): width(width), height(height) {
      
    }
    
    int width;
    int height;

    bool operator==(const FrameSize& b) const {
      return width == b.width && height == b.height;
    }
  };

  /**
   * Frame interval (frame rate).
   * You can compute frames per second using:
   * @code
   *  fps = numerator / denominator
   * @endcode
   */
  struct FrameInterval {
    FrameInterval(int numerator, int denominator) : numerator(numerator), denominator(denominator) {
      
    }
    
    int numerator;
    int denominator;
    
    bool operator==(const FrameInterval& b) const {
      return denominator == b.denominator && numerator == b.numerator;
    }
  };

  Camera();
  virtual ~Camera();

  void addCameraCallback(CameraCallback& callback);
  void removeCameraCallback(CameraCallback& callback);

  /**
   * Open needed resources
   */
  virtual int open() = 0;
  
  /**
   * Stop capture and close resources
   */
  virtual void close() = 0;

  /**
   * Start capturing frames
   *
   * @return 0 on success
   */
  virtual int start() = 0;

  /**
   * Wait until capture terminate
   */
  virtual void wait() = 0;

  /**
   * Get the list of supported pixel formats
   *
   * @param[out] pixelFormats supported pixel formats
   * @return 0 on success
   */
  virtual int getSupportedPixelFormats(std::vector<PixelFormat>& pixelFormats) = 0;
  
  /**
   * Get the list of supported frame sizes for a pixel format
   *
   * @param      pixelFormat
   * @param[out] frameSizes supported frame sizes
   * @return 0 on success
   */
  virtual int getSupportedFrameSizes(PixelFormat pixelFormat, std::vector<FrameSize>& frameSizes) = 0;
  
  /**
   * Get the list of supported frame intervals for a pixel format and a frame size
   *
   * @param      pixelFormat
   * @param      frameSize
   * @param[out] frameIntervals supported frame intervals
   * @return 0 on success
   */
  virtual int getSupportedFrameIntervals(PixelFormat pixelFormat, FrameSize& frameSize, std::vector<FrameInterval>& frameIntervals) = 0;

  virtual PixelFormat getPixelFormat() const = 0;
  virtual void setPixelFormat(PixelFormat pixelFormat) = 0;
  
  virtual FrameSize getFrameSize() const = 0;
  virtual void setFrameSize(FrameSize& frameSize) = 0;

  virtual FrameInterval getFrameInterval() const = 0;
  virtual void setFrameInterval(FrameInterval& frameInterval) = 0;

  /**
   * Print information about the device
   */
  virtual void printSummary() = 0;
  
protected:
  std::vector<CameraCallback*> callbacks;
  pthread_mutex_t callbacks_mutex;
};

}}

#endif // CAMERA_H
