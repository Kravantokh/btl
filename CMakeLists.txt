cmake_minimum_required(VERSION 3.22.2)

project(btl VERSION 1.0)

set(CMAKE_C_STANDARD 90)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Setting some variables for propagation from lower levels
set(BU_TERMINAL_MANAGER_INCLUDE /)
set(BU_UNICODE_INCLUDE /)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/benutils_terminal_manager)

add_executable(btl 
	${PROJECT_SOURCE_DIR}/src/btl.c
	)
target_link_libraries(btl bu_unicode bu_terminal_manager)

include_directories(${PROJECT_SOURCE_DIR}/include/ ${BU_TERMINAL_MANAGER_INCLUDE} ${BU_UNICODE_INCLUDE})
