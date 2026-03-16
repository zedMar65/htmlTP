#pragma once
#include <array>
#include <string>
#include <vector>
namespace htmlTP {

size_t vector22_hash(std::vector<std::array<int, 2>> *v);

class IsChars {
public:
  IsChars(const char *charsToRemove);

  bool operator()(char c);

private:
  const char *chars;
};

int id_gen();

void clear_name(std::string &name);

int parse_virtual_by_name(std::string parent_);

bool file_exists(std::string file_name);
}; // namespace htmlTP
