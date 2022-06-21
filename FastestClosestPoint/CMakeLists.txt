cmake_minimum_required(VERSION 3.16)
project(FastestClosestPoint)

set(CMAKE_CXX_STANDARD 14)

add_executable(fcpw.exe fcpw.cpp)
add_executable(obj_to_bod.exe obj_to_bod.cpp)

add_subdirectory(fcpw)

target_link_libraries(fcpw.exe fcpw)
target_include_directories(fcpw.exe PUBLIC ${FCPW_EIGEN_INCLUDES})
target_include_directories(fcpw.exe PUBLIC ${FCPW_ENOKI_INCLUDES})
