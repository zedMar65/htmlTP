#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"

bool htmlTP::htmlTemplate::render_lock() { return flags & RENDER_MASK; }
void htmlTP::htmlTemplate::set_render_lock(bool lock) {
  flags = (flags & ~RENDER_MASK) | (lock ? LOCKED : UNLOCKED);
}

bool htmlTP::htmlTemplate::template_state() {
  return (flags & TEMPLATE_STATE_MASK) >> 1;
}
void htmlTP::htmlTemplate::set_template_state(bool template_state) {
  flags = (flags & ~TEMPLATE_STATE_MASK) |
          (template_state ? FULL_TEMPLATE << 1 : NO_TEMPLATE << 1);
}

uint htmlTP::htmlTemplate::render_state() {
  return (flags & RENDER_STATE_MASK) >> 2;
}
void htmlTP::htmlTemplate::set_render_state(uint render_state) {
  flags = (flags & ~RENDER_STATE_MASK) | (render_state << 2);
}

uint htmlTP::htmlTemplate::type() { return (flags & TYPE_MASK) >> 4; }
void htmlTP::htmlTemplate::set_type(uint type) {
  flags = (flags & ~TYPE_MASK) | (type << 4);
}

uint32_t htmlTP::htmlTemplate::render_size() { return flags >> 8; }
void htmlTP::htmlTemplate::set_template_size(uint32_t size) {
  if (size > INT24_MAX) {
    throw std::overflow_error("Template size too large");
  }
  flags = (flags & LSB_MASK) | (size << 8);
}

std::string *htmlTP::htmlTemplate::name_handle() { return &name; }

std::string *htmlTP::htmlTemplate::file_handle() { return &file; }

std::unique_ptr<char[]> *htmlTP::htmlTemplate::tp_handle() { return &tp; }

std::unique_ptr<char[]> *htmlTP::htmlTemplate::render_handle() {
  return &render;
}

htmlTP::TP_handle htmlTP::get_TP_handle() {
  return std::make_unique<htmlTP::htmlTemplate>();
}
