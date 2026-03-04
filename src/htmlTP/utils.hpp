#pragma once
#include <string>
class IsChars {
public:
  IsChars(const char *charsToRemove);

  bool operator()(char c);

private:
  const char *chars;
};

int id_gen();

void clear_name(std::string &name);
