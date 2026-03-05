#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace htmlTP {

// Bitwise interaction flags
enum {
  LSB_MASK = 0xFF,

  VIRTUALITY_MASK = 0b11 << 6,
  TYPE_MASK = 0b11 << 4,
  RENDER_STATE_MASK = 0b11 << 2,
  TEMPLATE_STATE_MASK = 0b1 << 1,
  RENDER_MASK = 0b1,

  UNDEFINED = 0u,

  VIRT_RAW = 1u,
  VIRT_VIRTUAL = 2u,
  VIRT_FILE = 3u,

  HTML_TYPE = 1u,
  JS_TYPE = 2u,
  CSS_TYPE = 3u,

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

struct htmlTemplate;
struct Registry;
struct Parser;

struct htmlTP_state {
private:
  std::unique_ptr<Registry> registry;
  std::unique_ptr<Parser> parser;

public:
  htmlTP_state();
  ~htmlTP_state();

  int add_virtual_template(std::string name, std::string file_name,
                           uint32_t template_size = UNDEFINED,
                           uint type = UNDEFINED, const bool parse = true,
                           uint virtuality = UNDEFINED);

  void remove_template(const std::string name);

  htmlTemplate *get_template(const std::string name);

  bool exists(const std::string name);
};

using htmlTP_handle = std::unique_ptr<htmlTP_state>;

htmlTP_handle get_htmlTP_handle();

}; // namespace htmlTP
