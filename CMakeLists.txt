cmake_minimum_required(VERSION 3.26)
project(internet_disabler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math")

add_executable(internet_disabler main.cpp)
target_link_libraries(internet_disabler wlanapi)