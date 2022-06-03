cmake_minimum_required(VERSION 3.16)
project(benchmark)

set(CMAKE_CXX_STANDARD 14)

add_executable(fcpw.exe benchmark_fcpw.cpp)

add_subdirectory(fcpw)
target_link_libraries(fcpw.exe fcpw)
target_include_directories(fcpw.exe PUBLIC ${FCPW_EIGEN_INCLUDES})
target_include_directories(fcpw.exe PUBLIC ${FCPW_ENOKI_INCLUDES})
