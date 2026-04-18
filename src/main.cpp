#include <cstdio>

#include "config.h"
#include "shader.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

const double PHYSICS_INTERVAL_SECONDS = 1.0 / 60.0;
const double MAX_FRAME_TIME_SECONDS = 0.1;

const char *PROJECT_NAME = "Rainbow Triangle";

const float VERTICES[] = {
    // positions        // colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f,
    0.0f,  1.0f,  0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,  1.0f,
};

const char *VERTEX_SHADER_PATH = "assets/shaders/rainbow_triangle.vert";
const char *FRAGMENT_SHADER_PATH = "assets/shaders/rainbow_triangle.frag";

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

static bool initialize();
static void shutdown();
static bool process_input();
static void render(double alpha);
static void glfw_framebuffer_size_callback(GLFWwindow *window, int width,
                                           int height);
static void glfw_key_callback(GLFWwindow *window, int key, int scancode,
                              int action, int mods);
static void glfw_cursor_pos_callback(GLFWwindow *window, double xpos,
                                     double ypos);
static void glfw_mouse_button_callback(GLFWwindow *window, int button,
                                       int action, int mods);

struct {
  GLFWwindow *window;
  int framebuffer_width;
  int framebuffer_height;

  GLuint vao;
  GLuint vbo;
  ShaderProgram shader_program;

  bool exit_requested;
} g_state;

int main() {
  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  printf("%s v%d.%d.%d\n", PROJECT_NAME, VERSION_MAJOR, VERSION_MINOR,
         VERSION_PATCH);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(1280, 720, PROJECT_NAME, nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress))) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  glfwSwapInterval(0);

  g_state.window = window;

  glfwSetKeyCallback(window, glfw_key_callback);
  glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
  glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
  glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);

  if (!initialize()) {
    shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

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

    if (process_input()) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    while (accumulated_seconds >= PHYSICS_INTERVAL_SECONDS) {

      accumulated_seconds -= PHYSICS_INTERVAL_SECONDS;
    }

    double alpha = accumulated_seconds / PHYSICS_INTERVAL_SECONDS;
    render(alpha);
  }

  shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

bool initialize() {
  glfwGetFramebufferSize(g_state.window, &g_state.framebuffer_width, &g_state.framebuffer_height);
  glViewport(0, 0, g_state.framebuffer_width, g_state.framebuffer_height);

  g_state.shader_program = create_shader_program_from_files(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
  if (g_state.shader_program.id == 0) {
    fprintf(stderr, "Failed to create shader program from files\n");
    return false;
  }

  glGenVertexArrays(1, &g_state.vao);
  glGenBuffers(1, &g_state.vbo);

  glBindVertexArray(g_state.vao);
  glBindBuffer(GL_ARRAY_BUFFER, g_state.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return true;
}

void shutdown()
{
  if (g_state.vbo != 0) {
    glDeleteBuffers(1, &g_state.vbo);
    g_state.vbo = 0;
  }

  if (g_state.vao != 0) {
    glDeleteVertexArrays(1, &g_state.vao);
    g_state.vao = 0;
  }

  if (g_state.shader_program.id != 0) {
    delete_shader_program(g_state.shader_program);
    g_state.shader_program.id = 0;
    g_state.shader_program.uniform_locations.clear();
  }
}

bool process_input() {
  glfwPollEvents();

  if (g_state.exit_requested) {
    return true;
  }

  return false;
}

void render(double alpha) {
  (void) alpha;

  glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  bind_shader_program(g_state.shader_program);
  glBindVertexArray(g_state.vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
  unbind_shader_program();

  glfwSwapBuffers(g_state.window);
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  (void) window;
  g_state.framebuffer_width = width;
  g_state.framebuffer_height = height;
  glViewport(0, 0, width, height);
}

void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    g_state.exit_requested = true;
  }
}

void glfw_cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {}

void glfw_mouse_button_callback(GLFWwindow *window, int button, int action,
                                int mods) {}
