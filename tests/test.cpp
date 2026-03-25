#include "htmlTP/htmlTP.hpp"
#include <iostream>
using namespace htmlTP;
using namespace std;
int main() {
  char ab[50];
  char *a = &ab[0];
  a[40] = 'a';
  a[41] = 'b';
  int key = makeFourCC('a', 'b');
  cout << char(key);
  for (int i = 0; i < 50; i++) {
    char key_char = char(key);
    if (key_char == a[i]) {
      cout << to_string(i);
    }
  }

  return 0;
}
