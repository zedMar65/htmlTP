#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <sys/stat.h>

namespace htmlTP {

// Bitwise interaction flags
// | virtuality(2) | type(2) | render_state(2) | template_state | lock
void htmlTemplate::set_data(TP_data *data_) { data = *data_; }
TP_data *htmlTemplate::get_data() { return &data; }

uint htmlTemplate::lock() { return data.flags & LOCK_MASK; }
void htmlTemplate::set_lock(uint lock) {
  data.flags = (data.flags & ~LOCK_MASK) | lock;
}

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

char *htmlTemplate::tp_handle() { return tp.data.get(); }
char *htmlTemplate::render_handle() { return render.data.get(); }

// inequality in size means the size of template has been changed since last
// allocatio data ptr being nullptr means its unalocated/destroyed

char *htmlTemplate::alloc_tp() {
  if (data.template_size_ != tp.size) {
    free_tp();
  }
  // Aloc new if undefined or destroyed by free_tp
  if (tp.data == nullptr) {
    tp.data = std::make_unique<char[]>(data.template_size_);
  }

  return tp.data.get();
}

char *htmlTemplate::alloc_render() {
  if (data.render_size_ != render.size) {
    free_render();
  }
  if (render.data == nullptr) {
    render.data = std::make_unique<char[]>(data.render_size_);
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
