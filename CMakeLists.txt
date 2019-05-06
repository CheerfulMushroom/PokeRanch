cmake_minimum_required(VERSION 2.6)
project(prep-pokemon-ranch)
set(Tutorial_VERSION_MAJOR 1)
set(Tutorial_VERSION_MINOR 0)


set(PROJECT_SOURCE_DIR project/src)
set(PROJECT_HEADER_DIR project/include)

include_directories("${PROJECT_HEADER_DIR}")
add_executable(main.out ${PROJECT_SOURCE_DIR}/main.cpp
                        ${PROJECT_SOURCE_DIR}/states.cpp
                        ${PROJECT_SOURCE_DIR}/button.cpp)