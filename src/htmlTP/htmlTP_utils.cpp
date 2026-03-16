#include "htmlTP_utils.hpp"
#include "htmlTP/htmlTP.hpp"
#include <algorithm>
#include <array>
#include <random>
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

int id_gen() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist;

  return dist(gen);
}

void clear_name(std::string &name) {
  name.erase(std::remove_if(name.begin(), name.end(), IsChars("<\"\">")),
             name.end());
}

int parse_virtual_by_name(std::string parent_) {
  if (parent_.length() < 2) {
    return ERROR;
  }
  if (parent_[0] == '(' && parent_.back() == ')') {
    if (parent_[1] == '*') {
      return VIRT_LINK;
    }
    return VIRT_VIRTUAL;
  }
  if (file_exists(parent_)) {
    return VIRT_FILE;
  }
  return VIRT_RAW;
}

size_t vector22_hash(std::vector<std::array<int, 2>> *v) {
  size_t hash = 0;
  for (int i = 0; i < v->size(); i++) {
    hash = (hash + (324723947 + (*v)[i][0])) ^ 93485734985;
    hash = (hash + (324723947 + (*v)[i][1])) ^ 93485734985;
  }
  return hash;
}

}; // namespace htmlTP
