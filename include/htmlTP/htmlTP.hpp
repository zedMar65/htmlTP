#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace htmlTP {

// Bitwise interaction flags
enum {
  LSB_MASK = 0xFF,
  TYPE_MASK = 0b11 << 4,
  RENDER_STATE_MASK = 0b11 << 2,
  TEMPLATE_STATE_MASK = 0b1 << 1,
  RENDER_MASK = 0b1,

  UNDEFINED = 0u,

  HTML_TYPE = 1u,
  JS_TYPE = 2u,
  CSS_TYPE = 3u,

  NO_RENDER = 0u,
  PARTIAL_RENDER = 1u,
  FULL_RENDER = 2u,

  NO_TEMPLATE = 0u,
  FULL_TEMPLATE = 1u,

  UNLOCKED = 0u,
  LOCKED = 1u,

  INT24_MAX = 2147483647,
};
/*
 htmlTemplate{
        Rendering data
        Template data
        Ascosiated flags

        Interaction methods
* Notes
        Interaction with rendering and template buffers only through interaction
        methods that atheere to flags
 }
*/

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

struct htmlTP_state {
private:
  std::unordered_map<std::string, std::unique_ptr<htmlTP::htmlTemplate>>
      registry;

public:
  void add_template(const std::string *name, const std::string *file,
                    uint *type = nullptr);
  void add_virtual_template(const std::string *name, const char *render,
                            const uint render_size, const uint *type = nullptr);
  void remove_template(const std::string *name);
  htmlTP::htmlTemplate *get_template(const std::string *name);

  bool exists(const std::string *name);
};
using htmlTP_handle = std::unique_ptr<htmlTP::htmlTP_state>;

}; // namespace htmlTP
