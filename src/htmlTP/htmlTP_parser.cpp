#include "htmlTP_priv.hpp"
#include "htmlTP_utils.hpp"
#include <fstream>
#include <stdexcept>

namespace htmlTP {

Parser::Parser(Registry *registry_) { registry = registry_; }

void Parser::parse_TP(std::string name, bool force) {
  // TODO: write a parsing function
}

void Parser::read_TP(std::string name, const std::string data,
                     const bool re_parse) {
  // if reparse flag is set parsing is done auto:
  if (re_parse) {
    parse_TP(name, registry);
  }

  htmlTemplate *TP = registry->get_handle(name);

  if (TP->virtual_state() == VIRT_LINK && TP->parent_name() != "" &&
      registry != nullptr) {
    if (!registry->exists(TP->parent_name())) {
      throw std::runtime_error("No defined template reference " +
                               TP->parent_name());
    }
    TP->link_tp_buf(registry->get_handle(TP->parent_name())->get_render_link());
  }
  // Template read out of parent render
  if (TP->virtual_state() == VIRT_VIRTUAL && TP->parent_name() != "" &&
      registry != nullptr) {
    if (!registry->exists(TP->parent_name())) {
      throw std::runtime_error("No defined template reference " +
                               TP->parent_name());
    }

    *TP->alloc_tp() = *registry->get_handle(TP->parent_name())->render_handle();
    return;
  }

  // Template read out of file
  else if (TP->virtual_state() == VIRT_FILE && TP->parent_name() != "") {
    if (!file_exists(TP->parent_name())) {
      throw std::runtime_error("File " + TP->parent_name() + " not found");
    }
    std::fstream tp_file(TP->parent_name());
    tp_file.read(TP->alloc_tp(), TP->template_size());
    tp_file.close();
    return;
  }

  // Template read out of char array
  else if (TP->virtual_state() == VIRT_RAW && data != "") {
    TP->set_template_size(data.length());
    *TP->alloc_tp() = *data.c_str();
    return;
  }

  throw std::runtime_error("Could not resolve template source for: " + name);
}
}; // namespace htmlTP
