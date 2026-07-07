#include "htmlTP/htmlTP.hpp"
#include <cstring>
#include <iostream>
#include <memory>
using namespace htmlTP;
using namespace std;

char START_CLAUSE[] = "fg";
int main() {
  // TODO: write test to see if dependencies are created correctly
  // rawr
  htmlTP_handle handle = get_htmlTP_handle();
  int id = handle->add_const_template("some_template", "meow");
  char test_string[9] = "abcdefgh";
  for (int i = 0; i < 9; i++) {
    if (memcmp(START_CLAUSE, &(test_string[i]), 2) == 0) {
      cout << to_string(i) << endl;
    }
  }
  return 0;
}
