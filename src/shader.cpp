#include "shader.hpp"
#include "file_utils.hpp"

#include <cstdio>
#include <vector>
#include <glad/gl.h>

static void check_shader_compile_status(GLuint shader_id, const char *shader_type_name) {
  int success;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

  if (!success) {
    GLsizei info_log_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

    std::vector<char> info_log(info_log_length);

    glGetShaderInfoLog(shader_id, info_log_length, nullptr, info_log.data());
    fprintf(stderr, "Error compiling %s shader:\n%s\n", shader_type_name, info_log.data());
  }
}

static void check_program_link_status(GLuint program_id) {
  int success;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);

  if (!success) {
    GLsizei info_log_length;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

    std::vector<char> info_log(info_log_length);

    glGetProgramInfoLog(program_id, info_log_length, nullptr, info_log.data());
    fprintf(stderr, "Error linking shader program:\n%s\n", info_log.data());
  }
}

static const char *shader_type_name(GLenum shader_type) {
  switch (shader_type) {
    case GL_VERTEX_SHADER:
      return "vertex";
    case GL_FRAGMENT_SHADER:
      return "fragment";
    case GL_GEOMETRY_SHADER:
      return "geometry";
    default:
      return "unknown";
  }
}

static GLuint create_and_compile_shader(GLenum shader_type, const char *shader_source) {
  GLuint shader_id = glCreateShader(shader_type);

  glShaderSource(shader_id, 1, &shader_source, nullptr);
  glCompileShader(shader_id);

  check_shader_compile_status(shader_id, shader_type_name(shader_type));

  return shader_id;
}

ShaderProgram create_shader_program_from_source(const char *vertex_shader_source, const char *fragment_shader_source)
{
  GLuint vertex_shader_id, fragment_shader_id;
  vertex_shader_id = create_and_compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
  fragment_shader_id = create_and_compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

  std::vector<GLuint> shader_ids = {vertex_shader_id, fragment_shader_id};
  
  GLuint program_id = glCreateProgram();
  for (GLuint shader_id : shader_ids) {
    glAttachShader(program_id, shader_id);
  }
  glLinkProgram(program_id);

  check_program_link_status(program_id);

  for (GLuint shader_id : shader_ids) {
    glDeleteShader(shader_id);
  }

  std::unordered_map<std::string, int> uniform_locations;
  
  GLint active_uniform_count;
  glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &active_uniform_count);
  
  GLint max_uniform_name_length;
  glGetProgramiv(program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_name_length);
  if (max_uniform_name_length < 1) {
    max_uniform_name_length = 1;
  }

  std::vector<char> name_buffer(static_cast<size_t>(max_uniform_name_length));
  for (GLint i = 0; i < active_uniform_count; ++i) {
    GLsizei name_length;
    GLint size;
    GLenum type;

    glGetActiveUniform(program_id, i, max_uniform_name_length, &name_length, &size, &type, name_buffer.data());

    std::string uniform_name(name_buffer.data(), name_length);
    int location = glGetUniformLocation(program_id, uniform_name.c_str());

    uniform_locations[uniform_name] = location;
  }

  return ShaderProgram{program_id, uniform_locations};
}

ShaderProgram create_shader_program_from_files(const char *vertex_shader_path, const char *fragment_shader_path)
{
  std::string vertex_source;
  if (!read_entire_string_from_file(vertex_shader_path, vertex_source)) {
    fprintf(stderr, "Error reading vertex shader file: %s\n", vertex_shader_path);
    return ShaderProgram{0, {}};
  }

  std::string fragment_source;
  if (!read_entire_string_from_file(fragment_shader_path, fragment_source)) {
    fprintf(stderr, "Error reading fragment shader file: %s\n", fragment_shader_path);
    return ShaderProgram{0, {}};
  }

  return create_shader_program_from_source(vertex_source.c_str(), fragment_source.c_str());
}

void delete_shader_program(ShaderProgram &shader_program)
{
  glDeleteProgram(shader_program.id);
}

void bind_shader_program(ShaderProgram &shader_program)
{
  glUseProgram(shader_program.id);
}

void unbind_shader_program()
{
  glUseProgram(0);
}

int get_uniform_location(ShaderProgram &shader_program, const std::string &name)
{
  auto it = shader_program.uniform_locations.find(name);
  if (it != shader_program.uniform_locations.end()) {
    return it->second;
  } else {
    return -1;
  }
}

void set_uniform_float(ShaderProgram &shader_program, const std::string &name, float value)
{
  int location = get_uniform_location(shader_program, name);
  if (location != -1) {
    glUniform1f(location, value);
  }
}

void set_uniform_int(ShaderProgram &shader_program, const std::string &name, int value)
{
  int location = get_uniform_location(shader_program, name);
  if (location != -1) {
    glUniform1i(location, value);
  }
}

void set_uniform_float3(ShaderProgram &shader_program, const std::string &name, float x, float y, float z)
{
  int location = get_uniform_location(shader_program, name);
  if (location != -1) {
    glUniform3f(location, x, y, z);
  }
}

void set_uniform_int3(ShaderProgram &shader_program, const std::string &name, int x, int y, int z)
{
  int location = get_uniform_location(shader_program, name);
  if (location != -1) {
    glUniform3i(location, x, y, z);
  }
}
