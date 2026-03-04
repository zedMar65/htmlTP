#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <fstream>
#include <functional>
#include <stdexcept>
#include <sys/stat.h>

namespace htmlTP {

bool htmlTemplate::render_lock() { return flags & RENDER_MASK; }
void htmlTemplate::set_render_lock(bool lock) {
  flags = (flags & ~RENDER_MASK) | (lock ? LOCKED : UNLOCKED);
}

bool htmlTemplate::template_state() {
  return (flags & TEMPLATE_STATE_MASK) >> 1;
}
void htmlTemplate::set_template_state(bool template_state) {
  flags = (flags & ~TEMPLATE_STATE_MASK) |
          (template_state ? FULL_TEMPLATE << 1 : NO_TEMPLATE << 1);
}

uint htmlTemplate::render_state() { return (flags & RENDER_STATE_MASK) >> 2; }
void htmlTemplate::set_render_state(uint render_state) {
  flags = (flags & ~RENDER_STATE_MASK) | (render_state << 2);
}

uint htmlTemplate::virtual_state() { return (flags & VIRTUALITY_MASK) >> 6; }
void htmlTemplate::set_virtual_state(uint virtual_state_) {
  flags = (flags & ~VIRTUALITY_MASK) | (virtual_state_ << 6);
}

uint htmlTemplate::type() { return (flags & TYPE_MASK) >> 4; }
void htmlTemplate::set_type(uint type) {
  flags = (flags & ~TYPE_MASK) | (type << 4);
}

uint32_t htmlTemplate::render_size() { return render_size_; }
void htmlTemplate::set_render_size(uint32_t size) { render_size_ = size; }

uint32_t htmlTemplate::template_size() { return template_size_; }
void htmlTemplate::set_template_size(uint32_t size) { template_size_ = size; }

std::string htmlTemplate::parent_name() { return parent; }
void htmlTemplate::set_parent_name(std::string parent_name) {
  parent = parent_name;
}

std::unique_ptr<char[]> *htmlTemplate::tp_handle() { return &tp; }
std::unique_ptr<char[]> *htmlTemplate::render_handle() { return &render; }

TP_handle new_TP_handle() { return std::make_unique<htmlTemplate>(); }

// TODO: remove after propper definitions

void read_TP(htmlTemplate &TP, char *data, htmlTP_state *parent) {}

void parse_TP(htmlTemplate &TP, bool force, htmlTP_state *parent) {}

// TODO: move parsing functions to a different struct/class
// void parse_TP(htmlTemplate &TP, bool force, htmlTP_state *parent) {
//  // TODO: reparse only UNDEFINED/force
//}
//
// void read_TP(htmlTemplate &TP, char *data, htmlTP_state *parent) {
//  // TODO: move dependency of parent functions to registry functions
//  if (TP.virtual_state() == VIRT_VIRTUAL && *TP.file_handle() != "" &&
//      parent != nullptr) {
//    if (!parent->exists(*TP.file_handle())) {
//      throw std::runtime_error("No defined template reference " +
//                               *TP.file_handle());
//    }
//    TP.set_template_size(
//        parent->get_template(*TP.file_handle())->render_size());
//    *TP.tp_handle() = std::make_unique<char[]>(TP.template_size());
//    *TP.tp_handle()->get() = parent->get_render(TP.file_handle());
//    return;
//  }
//  if (TP.virtual_state() == VIRT_FILE && *TP.file_handle() != "") {
//    *TP.tp_handle() = std::make_unique<char[]>(TP.template_size());
//    std::fstream tp_file(*TP.file_handle());
//    tp_file.read(TP.tp_handle()->get(), TP.template_size());
//    tp_file.close();
//    return;
//  }
//  if (TP.virtual_state() == VIRT_RAW && data != nullptr) {
//    *TP.tp_handle() = std::make_unique<char[]>(TP.template_size());
//    *TP.tp_handle()->get() = *data;
//    return;
//  }
//
//  throw std::runtime_error("Could not resolve template source for: " +
//                           *TP.name_handle());
//}
}; // namespace htmlTP
