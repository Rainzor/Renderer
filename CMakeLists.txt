#---------------------------------------------------------------------------------------------------
# CMake Build Configuration for the Ray Tracing Weekend Series
#
# See README.md for guidance.
#---------------------------------------------------------------------------------------------------

cmake_minimum_required ( VERSION 3.1.0 )

project ( RayTracer
  VERSION 3.0.0
  LANGUAGES CXX
)
include_directories(common)
include_directories(include)
# Set to c++11
set ( CMAKE_CXX_STANDARD 11 )
# Executables
add_executable(main  main.cpp)

