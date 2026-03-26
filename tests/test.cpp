#include "htmlTP/htmlTP.hpp"
#include <iostream>
using namespace htmlTP;
using namespace std;

constexpr int makefourcc(char c1, char c2, char c3 = 0, char c4 = 0) {
  return (static_cast<int>(c1)) | (static_cast<int>(c2) << 8) |
         (static_cast<int>(c3) << 16) | (static_cast<int>(c4) << 24);
}

enum {
  START_CLAUSE = makefourcc('{', '!'),
  END_CLAUSE = makefourcc('!', '}'),
  CLAUSE_LENGTH = 2
};

string clause_to_string(int a, int len) {
  string b(len, ' ');
  for (int i = 0; i < len; i++) {
    b[i] = char((a >> i * 8) & 0xFFu);
  }
  return b;
}

int main() {
  char ab[50];
  char *a = &ab[0];
  a[40] = '{';
  a[41] = 'w';
  const char key = *clause_to_string(START_CLAUSE, CLAUSE_LENGTH).c_str();

  for (int i = 0; i < 50 - CLAUSE_LENGTH + 1; i++) {
    if (key == a[i]) {
      cout << to_string(i);
    }
  }

  return 0;
}
