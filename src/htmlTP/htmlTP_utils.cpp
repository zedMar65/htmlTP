#include "htmlTP_utils.hpp"
#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <random>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
namespace htmlTP {
IsChars::IsChars(const char *charsToRemove) : chars(charsToRemove) {};

bool IsChars::operator()(char c) {
  for (const char *testChar = chars; *testChar != 0; ++testChar) {
    if (*testChar == c) {
      return true;
    }
  }
  return false;
}

bool file_exists(std::string file_name) {

  struct stat sb;

  if (stat(file_name.c_str(), &sb) != 0) {
    return false;
  }
  return true;
}

int id_gen(std::string s) {
  std::hash<std::string> hasher;

  return (int)hasher(s);
}

// TODO: redo clear_name function
std::string clear_name(std::string &name) {
  name.erase(std::remove_if(name.begin(), name.end(), IsChars("<\"\">")),
             name.end());
  return name;
}

int parse_virtual_by_name(std::string parent_) {
  if (parent_.length() < 2) {
    return ERROR;
  }
  const std::string start_key = clause_to_string(START_CLAUSE, CLAUSE_LENGTH);
  const std::string end_key = clause_to_string(END_CLAUSE, CLAUSE_LENGTH);
  if (memcmp(parent_.c_str(), start_key.c_str(), CLAUSE_LENGTH) == 0 &&
      memcmp(&parent_.c_str()[parent_.size() - 1 - CLAUSE_LENGTH],
             end_key.c_str(), CLAUSE_LENGTH) == 0) {
    if (parent_[CLAUSE_LENGTH] == '*') {
      return VIRT_LINK;
    }
    return VIRT_VIRTUAL;
  }
  if (file_exists(parent_)) {
    return VIRT_FILE;
  }
  return VIRT_RAW;
}

std::string clause_to_string(int a, int len) {
  std::string b(len, ' ');
  for (int i = 0; i < len; i++) {
    b[i] = char((a >> i * 8) & 0xFFu);
  }
  return b;
}

std::string substr(char *arr, int begin, int len) {
  char *res = new char[len + 1];
  for (int i = 0; i < len; i++)
    res[i] = *(arr + begin + i);
  res[len] = 0;
  return res;
}

size_t vector22_hash(Compilation_commands *v) {
  size_t hash = 0;
  for (int i = 0; i < v->size(); i++) {
    hash = (hash + (324723947 + (*v)[i][0])) ^ 93485734985;
    hash = (hash + (324723947 + (*v)[i][1])) ^ 93485734985;
    hash = (hash + (324723947 + (*v)[i][2])) ^ 93485734985;
  }
  return hash;
}

bool isInteger(const std::string &s) {
  if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
    return false;

  char *p;
  strtol(s.c_str(), &p, 10);

  return (*p == 0);
}
}; // namespace htmlTP
