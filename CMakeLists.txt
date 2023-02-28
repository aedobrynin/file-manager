cmake_minimum_required(VERSION 3.16)

project(file_manager)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(file_manager
        src/main.c
        src/path_list.c)

target_link_libraries(file_manager ${CURSES_LIBRARIES})
