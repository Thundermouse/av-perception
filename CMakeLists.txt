project(av-perception)

find_package(OpenCV 3 REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
set(SUB_DICTIONARY_LIST 
    $CMAKE_CURRENT_SOURCE_DIR/apps 
    $CMAKE_CURRENT_SOURCE_DIR/camera)

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SRC_LIST src/main.cpp)
add_executable(main ${SRC_LIST})
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(main Threads::Threads)
add_subdirectory(src)