#include "htmlTP/htmlTP.hpp"
using namespace htmlTP;
using namespace std;
int main() {
  htmlTP_handle TP_handle = get_htmlTP_handle();
  TP_handle->add_virtual_template("some_name", "./hai.html");
  return 0;
}
