# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(NetPlay)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# # Make an interface library, and like OpenCV, GTSAm
# # and eigen to it
# add_library(ThirdParty INTERFACE)
# 
# find_package( OpenCV REQUIRED )
# include_directories( ${OpenCV_INCLUDE_DIRS} )
# message(STATUS "OpenCV version: ${OpenCV_VERSION}") 
# 
# find_package(GTSAM REQUIRED) # Uses installed package
# include_directories(${GTSAM_INCLUDE_DIR})
# message(STATUS "Found GTSAM version: ${GTSAM_VERSION}") 
# 
# find_package (Eigen3 3.3 REQUIRED NO_MODULE)
# message(STATUS "Found Eigen3 version: ${Eigen3_VERSION}")
# 
# target_link_libraries (ThirdParty INTERFACE Eigen3::Eigen gtsam ${OpenCV_LIBS})

add_subdirectory(src)
