add_subdirectory(log)
add_subdirectory(net)
add_subdirectory(tls)

add_executable(${CMAKE_PROJECT_NAME} botanio.cpp $<TARGET_OBJECTS:log> $<TARGET_OBJECTS:net> $<TARGET_OBJECTS:tls>)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${BOTAN_LIBRARIES})
