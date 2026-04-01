#include "htmlTP/htmlTP.hpp"
#include <cstring>
#include <iostream>
using namespace htmlTP;
using namespace std;

int main() {

  htmlTP_handle handle = get_htmlTP_handle();
  int id = handle->add_const_template("some_template", "meow");
  int id2 = handle->add_const_template("some", "meowmeow");

  return 0;
}
