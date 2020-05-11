#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>

__global__ static void greyRgbFlatArray(unsigned char *arrIn, unsigned char *arrOut, int imgPoints)
{
    int pointIndex = blockIdx.x * blockDim.x + threadIdx.x;
    if (3 * pointIndex < imgPoints)
    {
        unsigned char grey = 0.299f*arrIn[3*pointIndex] +  0.587f *arrIn[3*pointIndex + 1] + 0.114f *arrIn[3*pointIndex + 2];
        arrOut[pointIndex] = grey;
        arrOut[pointIndex + 1] = grey;
        arrOut[pointIndex + 2] = grey;
    }
}

void rgbFlatGrey(unsigned char* devPtr, unsigned char* HostIn, unsigned char* HostOut, int imgPoints)
{
    cudaMemcpy(devPtr, HostIn, 3 * imgPoints * sizeof(unsigned char), cudaMemcpyHostToDevice);
    greyRgbFlatArray<<<1, 1024>>>(devPtr, devPtr, imgPoints);
    cudaMemcpy(HostOut, devPtr, 3 * imgPoints * sizeof(unsigned char), cudaMemcpyDeviceToHost);
}
