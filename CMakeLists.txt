cmake_minimum_required(VERSION 3.22.2)

project(tm_launcher VERSION 1.0)

set(CMAKE_C_STANDARD 90)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Setting some variables for propagation from lower levels
set(BU_TERMINAL_MANAGER_INCLUDE /)
set(BU_UNICODE_INCLUDE /)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/benutils_terminal_manager)

add_executable(tm_launcher 
	${PROJECT_SOURCE_DIR}/src/tm_launcher.c
	)
target_link_libraries(tm_launcher bu_unicode bu_terminal_manager)

message(STATUS "tm dir: ${BU_TERMINAL_MANAGER_INCLUDE}")
message(STATUS "Unicode include dir: ${BU_UNICODE_INCLUDE}")

include_directories(${PROJECT_SOURCE_DIR}/include/ ${BU_TERMINAL_MANAGER_INCLUDE} ${BU_UNICODE_INCLUDE})
