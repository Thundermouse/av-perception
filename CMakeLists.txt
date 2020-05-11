cmake_minimum_required (VERSION 3.8)
project("av-perception" LANGUAGES CXX CUDA)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

add_subdirectory(src)
add_subdirectory(sample)