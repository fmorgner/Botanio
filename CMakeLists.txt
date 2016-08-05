cmake_minimum_required(VERSION 2.8)

# Setup generic project properties
project(botanio CXX)
add_definitions(-DASIO_STANDALONE)

# Include Conan packages
set(CONAN_BUILD_INFO_FILE ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
if(NOT EXISTS ${CONAN_BUILD_INFO_FILE})
  message(STATUS "No conan package information available. Trying to fix that!")
  execute_process(COMMAND conan install --build=missing ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_QUIET)
endif()

# Import dependencies via Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Set general compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -Werror")

# Ignore some harmless warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override -Wno-unused-local-typedef")

# Default to "Release" build
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "The configuration to build" FORCE)
endif()

# Define the output directory for 'Release' builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/products/Release/bin" CACHE PATH "" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/products/Release/lib" CACHE PATH "" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/products/Release/lib" CACHE PATH "" FORCE)

# Define the output directory for 'Debug' builds
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/products/Debug/bin" CACHE PATH "" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/products/Debug/lib" CACHE PATH "" FORCE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/products/Debug/lib" CACHE PATH "" FORCE)

# Search for the systems threading library
find_package(Threads REQUIRED)

# Tell CMake about our custom include paths
include_directories("include")

# Include the CMakeLists.txt found in the folder "src"
add_subdirectory(src)
