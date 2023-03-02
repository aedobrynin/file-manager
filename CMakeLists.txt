cmake_minimum_required(VERSION 3.16)

project(file_manager)

set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address,undefined")
add_compile_definitions(DEBUG=1)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(file_manager
        src/main.c
        src/path_list.c
        src/context.c
        src/menu_state.c
        src/filesystem.c)

target_link_libraries(file_manager ${CURSES_LIBRARY} menu)