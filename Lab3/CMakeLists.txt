cmake_minimum_required(VERSION 3.13)
project(LAB3_Server C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lrt -lm")


include_directories(.)

add_executable(LAB3_Server server.c list.c list.h wrapper.c wrapper.h)
add_executable(LAB3_Client client.c wrapper.c wrapper.h list.c list.h)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
target_include_directories(LAB3_Server PUBLIC ${GTK_INCLUDE_DIRS})
target_link_libraries(LAB3_Server ${GTK_LIBRARIES})
find_library(LIBRT rt)

target_link_libraries(LAB3_Server ${LIBRT})
target_link_libraries(LAB3_Client ${LIBRT})
target_link_libraries(LAB3_Server m)
target_link_libraries(LAB3_Client m)