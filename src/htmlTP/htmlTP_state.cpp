#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <memory>
#include <string>
#include <sys/stat.h>

namespace htmlTP {

htmlTP_state::~htmlTP_state() = default;

htmlTP_state::htmlTP_state() {

  registry = std::make_unique<Registry>();
  parser = std::make_unique<Parser>(registry.get());
}

// TODO: add a way to reasign/rebuild template to another VIRT type
// TODO: add a way to pass TP_data to the template
// TODO: parsing function frontend that would assign parsing data to handle

int htmlTP_state::add_virtual_template(std::string name,
                                       std::string parent_name, bool parse) {

  int id = registry->new_object(name);
  htmlTemplate *tp = registry->get_handle(id);

  tp->set_parent_name(parent_name);
  if (parse) {
    parser->parse_TP(name);
  }

  return id;
}

int htmlTP_state::add_const_template(const std::string name,
                                     const std::string data, const bool parse,
                                     const bool future_declare) {

  int id = registry->new_object(name);
  htmlTemplate *tp = registry->get_handle(id);

  const uint32_t data_len = data.length();
  TP_data tp_data_ = {"", 0, data_len, 0b0};
  tp->set_data(&tp_data_);

  parser->read_TP(name, data);

  if (parse) {
    parser->parse_compilation_commands(*tp, future_declare);
    parser->parse_dependency(*tp, future_declare);
  }

  return id;
}

htmlTP_handle get_htmlTP_handle() { return std::make_unique<htmlTP_state>(); }

}; // namespace htmlTP
