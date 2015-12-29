add_subdirectory(net)

add_executable(${CMAKE_PROJECT_NAME} botanio.cpp $<TARGET_OBJECTS:net>)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
