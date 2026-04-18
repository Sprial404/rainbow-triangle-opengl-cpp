#pragma once

#include <string>

/// @brief Reads an entire file into a string.
/// @param path Filesystem path to the file to read.
/// @param out_contents Output string populated with the file contents on success.
/// @return True if the file was read successfully; false if it could not be opened or read.
bool read_entire_string_from_file(const char *path, std::string &out_contents);
