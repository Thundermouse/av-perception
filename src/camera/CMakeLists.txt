cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11 )

find_package(OpenCV 3 REQUIRED)
find_package(CUDA REQUIRED)
find_package(Threads REQUIRED)

if(CUDA_FOUND)
    message(STATUS "CUDA library status")
    message(STATUS "    NVCC_EXECUTABLE: ${CUDA_NVCC_EXECUTABLE}")
    message(STATUS "    INCLUDE_PATH: ${CUDA_INCLUDE_DIRS}")
    message(STATUS "    TOOLKIT_DIR: ${CUDA_TOOLKIT_ROOT_DIR}")
    # Turning GPU
    set(CUDA_NVCC_FLAGS 
		"-gencode;arch=compute_75,code=sm_75;--std=C++;-G;-g"
        )
else(CUDA_FOUND)
    message("CUDA is not installed on this system")
endif()

if(OpenCV_FOUND)
    message(STATUS "OpenCV library status:")
    message(STATUS "    VERSION: ${OpenCV_VERSION}")
    message(STATUS "    LIBS: ${OpenCV_LIBS}")
    message(STATUS "    INCLUDE_PATH: ${OpenCV_INCLUDE_DIRS}")
else(OpenCV_FOUND)
    message("OpenCV is not installed on this system")
endif()

set(CUDA_KERNALS
    cudaKernal/cameraProcess.cu
)

set(CUDA_HEADERS
    ${CMAKE_SOURCE_DIR}/include/cameraProcess.h
)

set(CUDA_SRCS
    cameraProcess.cpp
)

add_library(libcudacv STATIC 
        ${CUDA_KERNALS} 
        ${CUDA_HEADERS} 
        ${CUDA_SRCS}
)

target_include_directories( libcudacv PUBLIC ${CUDA_INCLUDE_DIRS})
target_include_directories( libcudacv PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(libcudacv ${OpenCV_LIBS})
#target_link_libraries(libcudacv Threads::Threads)
