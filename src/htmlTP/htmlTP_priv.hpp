#pragma once
#include <memory>
#include <string>
namespace htmlTP {

struct htmlTemplate {
private:
  int _id;
  std::string name;
  std::string file;
  uint render_size_;
  uint template_size_;
  // | 0x00 | type(2) | render_state(2) | template_state
  // | render_lock
  //  render_size is 2^24 bits
  uint32_t flags = 0b0;
  // Compilation output
  std::unique_ptr<char[]> render;
  // Compilation instructions
  std::unique_ptr<char[]> tp;

public:
  int *id();
  int *set_id();

  bool render_lock();
  void set_render_lock(bool lock);

  bool template_state();
  void set_template_state(bool template_state);

  uint render_state();
  void set_render_state(uint render_state);

  uint type();
  void set_type(uint type);

  uint32_t render_size();
  void set_render_size(uint32_t size);

  uint32_t template_size();
  void set_template_size(uint32_t size);

  std::string *name_handle();
  std::string *file_handle();

  std::unique_ptr<char[]> *tp_handle();
  std::unique_ptr<char[]> *render_handle();
};

using TP_handle = std::unique_ptr<htmlTP::htmlTemplate>;

htmlTP::TP_handle get_TP_handle();

void parse_TP(htmlTemplate &TP);
void read_TP(htmlTemplate &TP, char *data[]);
}; // namespace htmlTP
