cmake_minimum_required(VERSION 2.6)
project(prep-pokemon-ranch)
set(Tutorial_VERSION_MAJOR 1)
set(Tutorial_VERSION_MINOR 0)


set(PROJECT_SOURCE_DIR project/src)
set(PROJECT_HEADER_DIR project/include)

include_directories("${PROJECT_HEADER_DIR}")
find_package(OpenCV REQUIRED )

add_executable(main.out
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/Game.cpp
        ${PROJECT_SOURCE_DIR}/GameWindow.cpp
        ${PROJECT_SOURCE_DIR}/ShaderProgram.cpp
        ${PROJECT_SOURCE_DIR}/States.cpp
        ${PROJECT_SOURCE_DIR}/Button.cpp
        ${PROJECT_SOURCE_DIR}/ButtonFunctions.cpp
        ${PROJECT_SOURCE_DIR}/VideoStream.cpp project/include/Interfaces.h)

target_link_libraries(main.out
        GLEW
        glfw
        GL
        X11
        pthread
        Xrandr
        Xi
        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_imgcodecs
        opencv_videoio
        aruco)
