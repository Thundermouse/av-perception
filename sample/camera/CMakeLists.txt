find_package(OpenCV 3 REQUIRED)
find_package(Threads REQUIRED)
find_package(CUDA REQUIRED)
find_package(CUFRAMEWORK REQUIRED )

cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

add_executable(sample_app "main.hpp" "main.cpp")
target_link_libraries(sample_app libcudacv)
target_link_libraries(sample_app Threads::Threads)