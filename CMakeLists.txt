cmake_minimum_required(VERSION 4.1 FATAL_ERROR)
project(rainbow-triangle C CXX)

include(cmake/safeguards.cmake)

message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

include(cmake/git_revision.cmake)
include(cmake/dependencies.cmake)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/config.h.in
    ${PROJECT_BINARY_DIR}/config.h
    )

add_executable(rainbow-triangle src/main.cpp)
target_include_directories(
    rainbow-triangle
    PRIVATE
        include
        ${PROJECT_BINARY_DIR}
    )
target_link_libraries(rainbow-triangle
    PRIVATE
        glad
        glfw
    )