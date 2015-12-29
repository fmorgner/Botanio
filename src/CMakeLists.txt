add_subdirectory(net)

add_executable(${CMAKE_PROJECT_NAME} botanio.cpp $<TARGET_OBJECTS:net>)
