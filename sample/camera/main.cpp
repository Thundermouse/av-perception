#include "main.hpp"

std::atomic_bool gRun;

void (*gTerminateFunc)(void) = [] {gRun = false;std::cout<<"system exit."<<std::endl; };
void (*gTerminateSalHandler)(int sig) = [](int sig) { gTerminateFunc(); };
const int RGB_CHANNEL_NUM = 3;
const int maxPoolSize = 1920 * 1080 * 3;

void cameraCapatureLoop()
{
    cv::VideoCapture capture(0);
    if (capture.isOpened())
    {
        capture.set(cv::CAP_PROP_MODE, cv::CAP_MODE_YUYV);
        std::cout << "get camera success." << std::endl;
    }
    cv::Mat frame;
    unsigned char *deviceInOut = nullptr;

    cudaError_t err = cudaMalloc((void **)&deviceInOut, maxPoolSize * sizeof(unsigned char));

    if (err != cudaSuccess)
    {
        printf("cudaMalloc : %s\n", cudaGetErrorString(cudaGetLastError()));
        exit(EXIT_FAILURE);
    }
    while (capture.isOpened() && gRun == true)
    {
        capture >> frame;
        int imgSize = frame.cols * frame.rows;
        /*
        for (int pointIndex = 0; pointIndex < imgSize; ++pointIndex)
        {
            unsigned char grey = 0.299f * frame.data[3 * pointIndex] + 0.587f * frame.data[3 * pointIndex + 1] + 0.114f * frame.data[3 * pointIndex + 2];
            frame.data[3*pointIndex] = grey;
            frame.data[3*pointIndex + 1] = grey;
            frame.data[3*pointIndex + 2] = grey;
        }
        */
        if (imgSize <= maxPoolSize)
        {
            //cuFrameWork::cuFrameWorkCalGrey(deviceInOut, frame.data, frame.data, imgSize);
        }
        cv::imshow("capture", frame);

        char key = static_cast<char>(cv::waitKey(33)); //控制视频流的帧率，10ms一帧
        if (key == 27)                                 //按esc退出
            gTerminateFunc();
    }

    cudaFree(deviceInOut);
}

void regesterSignal()
{
    struct sigaction sa
    {
    };
    sa.sa_handler = gTerminateSalHandler;
    sa.sa_flags = false;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, nullptr) == -1)
    {
        std::cout << "register signal Fail" << std::endl;
    }
}

int main()
{
    gRun = true;
    std::thread cameraCapatureThread(cameraCapatureLoop);
    regesterSignal();
    cameraCapatureThread.join();
    //rgbFlatGrey(nullptr, nullptr, nullptr, 1);
    return 0;
}
