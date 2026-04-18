# Provides the required dependencies for the project.

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME GLFW
  GITHUB_REPOSITORY glfw/glfw
  GIT_TAG 3.4
  OPTIONS
    "GLFW_BUILD_EXAMPLES OFF"
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_DOCS OFF"
)

add_library(glad STATIC vendor/glad/src/gl.c)
target_include_directories(
    glad
    PUBLIC
        vendor/glad/include
)