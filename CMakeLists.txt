cmake_minimum_required(VERSION 3.20.0)
project(Snake++)

find_package(Curses REQUIRED)

add_executable(Snake++ src/main.cpp)
target_link_libraries(Snake++ ${CURSES_LIBRARIES})
