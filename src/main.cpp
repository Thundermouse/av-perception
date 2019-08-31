#include <iostream>
#include <thread>
#include <opencv2/videoio.hpp>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <string>
#include <atomic> 

// system.h
std::atomic_bool sRun = true;

// utils.h
struct vector3d
{
    int x;
    int y;
    int z;
};

// image.h
class darkNetImage
{
private:
    vector3d m_size{};
    float *m_data = nullptr;

public:
    void setImage(const IplImage &src)
    {
        if (m_size.x != src.height ||
            m_size.y != src.width ||
            m_size.z != src.nChannels)
        {
            m_size.x = src.height;
            m_size.y = src.width;
            m_size.z = src.nChannels;

            if (m_data)
            {
                delete m_data;
                m_data = nullptr;
            }
        }

        if (m_data == nullptr)
        {
            m_data = new float[m_size.x * m_size.y * m_size.z * sizeof(float)];
        }
        size_t mat = m_size.x * m_size.y;
        const size_t &col = m_size.y;
        const size_t &chn = m_size.z;
        unsigned char* oriData = (unsigned char*)(src.imageData);
        for (size_t z = 0; z < m_size.z; ++z)
    {
            for (size_t x = 0; x < m_size.x; ++x)
            {
                for (size_t y = 0; y < m_size.y; ++y)
                    m_data[z * mat + x * col + y] = 
                        oriData[x * src.widthStep + y * chn + z]/255;
            }
            std::cout<<z<<std::endl;
        }
    }

    const vector3d &getSize() const
    {
        return m_size;
    }

    const float *getDataPtr() const
    {
        return m_data;
    }

    darkNetImage(){};

    darkNetImage(const IplImage &src)
    {
        setImage(src);
    }

    ~darkNetImage()
    {
        if (m_data)
        {
            delete m_data;
        }
    }
};

class videoImage
{
public:
    static videoImage &getObject()
    {
        static videoImage instance;
        return instance;
    }

    void setPstFrame(const darkNetImage &frame)
    {
        std::lock_guard<std::mutex> lock(m_frameLock);
        m_pstFrame = frame;
    }

    void setPstFrame(const IplImage &frame)
    {
        std::lock_guard<std::mutex> lock(m_frameLock);
        m_pstFrame.setImage(frame);
    }

    const darkNetImage &getPstFrame()
    {
        std::lock_guard<std::mutex> lock(m_frameLock);
        return m_pstFrame;
    }

    struct object_creator
    {
        object_creator()
        {
            videoImage::getObject();
        }
    };
    static object_creator _create_object;

    videoImage(const videoImage &) = delete;
    void operator=(const videoImage &) = delete;

private:
    videoImage(){};
    darkNetImage m_pstFrame;

    std::mutex m_frameLock;
};

void captureImageLoop()
{
    cv::VideoCapture capture(0);
    if (capture.isOpened())
    {
        //capture.set(cv::CAP_PROP_MODE, cv::CAP_MODE_YUYV);
        std::cout << "success" << std::endl;
    }
    cv::Mat matFrame;
    while (capture.isOpened() && sRun)
    {
        capture >> matFrame;
        IplImage pstImage = matFrame;
        videoImage &pstVideoImage = videoImage::getObject();
        pstVideoImage.setPstFrame(pstImage);
        cv::imshow("capture", matFrame);
        char key = static_cast<char>(cv::waitKey(33));
        if (key == 27)
        {
            sRun = false;
            break;
        }
    }
}

void detectObjectLoop()
{
    while(sRun)
    {
        std::cout<<"TODO dranetNet logic"<<std::endl;
    }
}

// initilize singleton
videoImage::object_creator videoImage::_create_object;

//main
int main(int argc, char **argv)
{
    std::thread cameraThread(captureImageLoop);
    std::thread detectThread(detectObjectLoop);

    detectThread.join();
    cameraThread.join();
    return 0;
}