#include "file_utils.hpp"

#include <fstream>
#include <sstream>

bool read_entire_string_from_file(const char *path, std::string &out_contents)
{
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    return false;
  }

  std::ostringstream stream;
  stream << file.rdbuf();

  if (file.bad()) {
    return false;
  }

  out_contents = stream.str();
  return true;
}
