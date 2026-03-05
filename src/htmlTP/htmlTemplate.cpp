#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
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
void htmlTemplate::set_render_size(size_t size) { render_size_ = size; }

uint32_t htmlTemplate::template_size() { return template_size_; }

void htmlTemplate::set_template_size(size_t size) { template_size_ = size; }

std::string htmlTemplate::parent_name() { return parent; }
void htmlTemplate::set_parent_name(std::string parent_name) {
  parent = parent_name;
}

char *htmlTemplate::tp_handle() { return tp.data.get(); }
char *htmlTemplate::render_handle() { return render.data.get(); }

// inequality in size means the size of template has been changed since last
// allocatio data ptr being nullptr means its unalocated/destroyed

char *htmlTemplate::alloc_tp() {
  if (template_size_ != tp.size) {
    free_tp();
  }
  // Aloc new if undefined or destroyed by free_tp
  if (tp.data == nullptr) {
    tp.data = std::make_unique<char[]>(template_size_);
  }

  return tp.data.get();
}

char *htmlTemplate::alloc_render() {
  if (render_size_ != render.size) {
    free_render();
  }
  if (render.data == nullptr) {
    render.data = std::make_unique<char[]>(render_size_);
  }
  return render.data.get();
}

void htmlTemplate::free_tp() {
  tp.data.reset();
  tp.data = nullptr;
  tp.size = 0;
}

void htmlTemplate::free_render() {
  render.data.reset();
  render.data = nullptr;
  render.size = 0;
}

TP_handle new_TP_handle() { return std::make_unique<htmlTemplate>(); }

}; // namespace htmlTP
