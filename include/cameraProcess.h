#pragma once
#include <cuda.h>
#include <cuda_runtime.h>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>

void rgbFlatGrey(unsigned char* devPtr, unsigned char* HostIn, unsigned char* HostOut, int imgSize);

namespace cuFrameWork
{
    void cuFrameWorkCalGrey(unsigned char* devPtr, unsigned char* HostIn, unsigned char* HostOut, int imgSize);
}