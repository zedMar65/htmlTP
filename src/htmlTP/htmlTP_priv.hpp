#pragma once
#include <memory>
#include <string>
namespace htmlTP {

struct htmlTemplate {
private:
  std::string name;
  std::string file;

  // render_size(24) | 00 | type(2) | render_state(2) | template_state
  // | render_lock
  //  render_size is 2^24 bits
  uint32_t flags = 0b0;
  // Compilation output
  std::unique_ptr<char[]> render;
  // Compilation instructions
  std::unique_ptr<char[]> tp;

public:
  bool render_lock();
  void set_render_lock(bool lock);

  bool template_state();
  void set_template_state(bool template_state);

  uint render_state();
  void set_render_state(uint render_state);

  uint type();
  void set_type(uint type);

  uint32_t render_size();
  void set_template_size(uint32_t size);

  std::string *name_handle();

  std::string *file_handle();

  std::unique_ptr<char[]> *tp_handle();

  std::unique_ptr<char[]> *render_handle();
};

using TP_handle = std::unique_ptr<htmlTP::htmlTemplate>;

htmlTP::TP_handle get_TP_handle();
}; // namespace htmlTP
