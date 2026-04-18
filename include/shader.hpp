#pragma once

#include <unordered_map>
#include <string>
#include <glad/gl.h>

struct ShaderProgram {
  GLuint id;
  std::unordered_map<std::string, int> uniform_locations;
};

/// @brief Creates and links a shader program from vertex/fragment shader source strings.
/// @param vertex_shader_source Null-terminated GLSL source code for the vertex shader.
/// @param fragment_shader_source Null-terminated GLSL source code for the fragment shader.
/// @return A ShaderProgram handle that can be bound and used for rendering.
/// @note Any shader compilation or program linking diagnostics are printed to stderr.
ShaderProgram create_shader_program_from_source(const char *vertex_shader_source, const char *fragment_shader_source);

/// @brief Creates and links a shader program from vertex/fragment shader source files.
/// @param vertex_shader_path Filesystem path to the vertex shader GLSL source file.
/// @param fragment_shader_path Filesystem path to the fragment shader GLSL source file.
/// @return A ShaderProgram handle that can be bound and used for rendering.
/// @note Any shader compilation or program linking diagnostics are printed to stderr.
ShaderProgram create_shader_program_from_files(const char *vertex_shader_path, const char *fragment_shader_path);

/// @brief Deletes the shader program object and frees associated GPU resources.
/// @param shader_program The shader program to delete.
void delete_shader_program(ShaderProgram &shader_program);

/// @brief Binds the shader program for subsequent draw calls.
/// @param shader_program The shader program to bind.
void bind_shader_program(ShaderProgram &shader_program);

/// @brief Unbinds any currently bound shader program by binding program 0.
void unbind_shader_program();

/// @brief Gets the location of a uniform variable in the shader program.
/// @param shader_program The shader program to query.
/// @param name The name of the uniform variable.
/// @return The location of the uniform variable, or -1 if not found.
int get_uniform_location(ShaderProgram &shader_program, const std::string &name);

/// @brief Sets a float uniform variable in the shader program.
/// @param shader_program The shader program to update.
/// @param name The name of the uniform variable.
/// @param value The float value to set.
void set_uniform_float(ShaderProgram &shader_program, const std::string &name, float value);

/// @brief Sets an int uniform variable in the shader program.
/// @param shader_program The shader program to update.
/// @param name The name of the uniform variable.
/// @param value The int value to set.
void set_uniform_int(ShaderProgram &shader_program, const std::string &name, int value);

/// @brief Sets a vec3 uniform variable in the shader program.
/// @param shader_program The shader program to update.
/// @param name The name of the uniform variable.
/// @param x The x component of the vec3 value.
/// @param y The y component of the vec3 value.
/// @param z The z component of the vec3 value.
void set_uniform_float3(ShaderProgram &shader_program, const std::string &name, float x, float y, float z);

/// @brief Sets an ivec3 uniform variable in the shader program.
/// @param shader_program The shader program to update.
/// @param name The name of the uniform variable.
/// @param x The x component of the ivec3 value.
/// @param y The y component of the ivec3 value.
/// @param z The z component of the ivec3 value.
void set_uniform_int3(ShaderProgram &shader_program, const std::string &name, int x, int y, int z);