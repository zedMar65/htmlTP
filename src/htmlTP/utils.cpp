#include "utils.hpp"
#include <algorithm>
#include <random>
IsChars::IsChars(const char *charsToRemove) : chars(charsToRemove) {};

bool IsChars::operator()(char c) {
  for (const char *testChar = chars; *testChar != 0; ++testChar) {
    if (*testChar == c) {
      return true;
    }
  }
  return false;
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
