#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <string>
int main()
{
    cv::VideoCapture capture(0);
    if(capture.isOpened())
    {
        //capture.set(cv::CAP_PROP_MODE, cv::CAP_MODE_YUYV);
        std::cout<<"success"<<std::endl;
    }
    cv::Mat frame;
    while (capture.isOpened())
    {
        capture >> frame;
        cv::imshow("capture", frame);
        char key = static_cast<char>(cv::waitKey(33));//控制视频流的帧率，10ms一帧
        if (key == 27)  //按esc退出
            break;               
    }
    return 0;
}
