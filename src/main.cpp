#include <cstdio>

#include "config.h"
#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}

static void process_input();
static void render(double alpha);
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

const double PHYSICS_INTERVAL_SECONDS = 1.0 / 120.0;
const double MAX_FRAME_TIME_SECONDS = 0.1;

const char *PROJECT_NAME = "Rainbow Triangle";

struct {

} g_state;

int main() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  printf("%s v%d.%d.%d\n", PROJECT_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(1280, 720, PROJECT_NAME, nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  glfwSetKeyCallback(window, glfw_key_callback);
  glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
  glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  double accumulated_seconds = 0.0;
  double previous_time_seconds = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    double current_time_seconds = glfwGetTime();
    double frame_elapsed_seconds = current_time_seconds - previous_time_seconds;
    previous_time_seconds = current_time_seconds;

    if (frame_elapsed_seconds > MAX_FRAME_TIME_SECONDS) {
      frame_elapsed_seconds = MAX_FRAME_TIME_SECONDS;
    }
    accumulated_seconds += frame_elapsed_seconds;

    process_input();

    while (accumulated_seconds >= PHYSICS_INTERVAL_SECONDS) {
      
      accumulated_seconds -= PHYSICS_INTERVAL_SECONDS;
    }

    double alpha = accumulated_seconds / PHYSICS_INTERVAL_SECONDS;
    render(alpha);
  }

  return 0;
}

void process_input()
{
}

void glfw_key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
}

void glfw_cursor_pos_callback(GLFWwindow * window, double xpos, double ypos)
{
}

void glfw_mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
}
