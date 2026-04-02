#pragma once
#include "htmlTP_priv.hpp"
#include <array>
#include <string>
#include <vector>
namespace htmlTP {

size_t vector22_hash(Compilation_commands *v);

class IsChars {
public:
  IsChars(const char *charsToRemove);

  bool operator()(char c);

private:
  const char *chars;
};

int id_gen(std::string s);

std::string clear_name(std::string &name);

int parse_virtual_by_name(std::string parent_);

bool file_exists(std::string file_name);

std::string substr(char *arr, int begin, int len);

std::string clause_to_string(int a, int len);
bool isInteger(const std::string &s);
}; // namespace htmlTP
