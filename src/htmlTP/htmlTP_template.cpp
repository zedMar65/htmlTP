#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include "htmlTP_utils.hpp"
#include <array>
#include <iostream>
#include <sys/stat.h>
#include <vector>

namespace htmlTP {
// Bitwise interaction flags
// | virtuality(2) | type(2) | render_state(2) | template_state | lock
void htmlTemplate::set_data(TP_data *data_) { data = *data_; }
TP_data *htmlTemplate::get_data() { return &data; }

uint htmlTemplate::lock() { return data.flags & LOCK_MASK; }
void htmlTemplate::set_lock(uint lock) {
  data.flags = (data.flags & ~LOCK_MASK) | lock;
}

// INFO:  set_render_hash should be called on any final change of render.data
// template hash likewise
void htmlTemplate::set_render_hash() {
  std::hash<std::string_view> hash_f;
  data.render_hash_ = hash_f(render__->data.get());
}

void htmlTemplate::set_template_hash() {
  std::hash<std::string_view> hash_f;
  data.template_hash_ = hash_f(tp__->data.get());
}

void htmlTemplate::set_compilaion_hash() {
  data.compilation_hash = vector22_hash(&compilation_commands);
}

int *htmlTemplate::render_hash_handle() { return &data.render_hash_; }
int *htmlTemplate::template_hash_handle() { return &data.template_hash_; }

uint htmlTemplate::template_state() {
  return (data.flags & TEMPLATE_STATE_MASK);
}
void htmlTemplate::set_template_state(uint template_state) {
  data.flags = (data.flags & ~TEMPLATE_STATE_MASK) | template_state;
}

uint htmlTemplate::render_state() { return (data.flags & RENDER_STATE_MASK); }
void htmlTemplate::set_render_state(uint render_state) {
  data.flags = (data.flags & ~RENDER_STATE_MASK) | render_state;
}

uint htmlTemplate::virtual_state() { return (data.flags & VIRTUALITY_MASK); }
void htmlTemplate::set_virtual_state(uint virtual_state_) {
  data.flags = (data.flags & ~VIRTUALITY_MASK) | virtual_state_;
}

uint htmlTemplate::type() { return (data.flags & TYPE_MASK); }
void htmlTemplate::set_type(uint type) {
  data.flags = (data.flags & ~TYPE_MASK) | type;
}

uint32_t htmlTemplate::render_size() { return data.render_size_; }
void htmlTemplate::set_render_size(size_t size) { data.render_size_ = size; }

uint32_t htmlTemplate::template_size() { return data.template_size_; }

void htmlTemplate::set_template_size(size_t size) {
  data.template_size_ = size;
}

std::string htmlTemplate::parent_name() { return data.parent; }
void htmlTemplate::set_parent_name(std::string parent_name) {
  data.parent = parent_name;
}

char *htmlTemplate::tp_handle() { return tp__->data.get(); }
char *htmlTemplate::render_handle() { return render__->data.get(); }

// WARNING: linking is strictly for VIRT_LINK and should be explisitly unlinked
// after changing type
void htmlTemplate::link_tp_buf(Buffer *buf) {
  free_tp();
  tp__ = buf;
}
Buffer *htmlTemplate::get_render_link() { return render__; }

Compilation_commands *htmlTemplate::compilation_commands_handle() {
  return &compilation_commands;
}

void htmlTemplate::unlink_tp_buf() { tp__ = &tp_default; }
// inequality in size means the size of template has been changed since last
// allocatio data ptr being nullptr means its unalocated/destroyed

char *htmlTemplate::alloc_tp() {
  if (data.template_size_ != tp__->size) {
    free_tp();
  }
  // Aloc new if undefined or destroyed by free_tp
  if (tp__->data == nullptr) {
    tp__->data = std::make_unique<char[]>(data.template_size_);
  }

  return tp__->data.get();
}

char *htmlTemplate::alloc_render() {
  if (data.render_size_ != render__->size) {
    free_render();
  }
  if (render__->data == nullptr) {
    render__->data = std::make_unique<char[]>(data.render_size_);
  }
  return render__->data.get();
}

void htmlTemplate::free_tp() {
  tp__->data.reset();
  tp__->data = nullptr;
  tp__->size = 0;
}

void htmlTemplate::free_render() {
  render__->data.reset();
  render__->data = nullptr;
  render__->size = 0;
}

TP_handle new_TP_handle() { return std::make_unique<htmlTemplate>(); }
}; // namespace htmlTP
