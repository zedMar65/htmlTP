#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <functional>
#include <sys/stat.h>

namespace htmlTP {
int *htmlTemplate::id() { return &_id; }

int *htmlTemplate::set_id() {
  std::hash<std::string> hshr;
  _id = hshr(name);
  return &_id;
}

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

uint htmlTemplate::type() { return (flags & TYPE_MASK) >> 4; }
void htmlTemplate::set_type(uint type) {
  flags = (flags & ~TYPE_MASK) | (type << 4);
}

uint32_t htmlTemplate::render_size() { return render_size_; }
void htmlTemplate::set_render_size(uint32_t size) { render_size_ = size; }

uint32_t htmlTemplate::template_size() { return template_size_; }
void htmlTemplate::set_template_size(uint32_t size) { template_size_ = size; }

std::string *htmlTemplate::name_handle() { return &name; }

std::string *htmlTemplate::file_handle() { return &file; }

std::unique_ptr<char[]> *htmlTemplate::tp_handle() { return &tp; }

std::unique_ptr<char[]> *htmlTemplate::render_handle() { return &render; }

TP_handle get_TP_handle() { return std::make_unique<htmlTemplate>(); }

void parse_TP(htmlTemplate &TP) {}
void read_TP(htmlTemplate &TP, char *data) {

  if (data == nullptr) {
  } else {
    *TP.tp_handle() = std::make_unique<char[]>(TP.template_size());
    *TP.tp_handle()->get() = *data;
  }
}

}; // namespace htmlTP
