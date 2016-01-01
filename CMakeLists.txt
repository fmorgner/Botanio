cmake_minimum_required(VERSION 2.8)

# Setup generic project properties
project(botanio CXX)
add_definitions(-DASIO_STANDALONE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Default to "Release" build
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "The configuration to build" FORCE)
endif()

# Define the output directory for our products
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/products/${CMAKE_BUILD_TYPE}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/products/${CMAKE_BUILD_TYPE}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/products/${CMAKE_BUILD_TYPE}/lib")

# Tell CMake about our custom modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Include the FindBotan module
include(FindBotan)

# Actually search for botan
find_package(Botan REQUIRED)

# Search for the systems threading library
find_package(Threads REQUIRED)

# Tell CMake about our custom include paths
include_directories("include")
include_directories("include/third_party/asio")
include_directories(${BOTAN_INCLUDE_DIRS})

# Include the CMakeLists.txt found in the folder "src"
add_subdirectory(src)
