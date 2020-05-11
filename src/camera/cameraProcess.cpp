#include "cameraProcess.h"

namespace cuFrameWork
{
void cuFrameWorkCalGrey(unsigned char* devPtr, unsigned char* HostIn, unsigned char* HostOut, int imgSize)
{
    rgbFlatGrey(devPtr, HostIn, HostOut, imgSize);
    cudaDeviceSynchronize();
}
}
