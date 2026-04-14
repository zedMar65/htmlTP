#pragma once

#include <memory>
#include <string>

namespace htmlTP {

// Bitwise interaction flags for htmlTP object
// | virtuality(2) | type(2) | render_state(2) | template_state | lock
enum {
  VIRTUALITY_MASK = 0b11 << 6,
  TYPE_MASK = 0b11 << 4,
  RENDER_STATE_MASK = 0b11 << 2,
  TEMPLATE_STATE_MASK = 0b1 << 1,
  LOCK_MASK = 0b1,

  UNDEFINED = 0u,
  ERROR = -1,

  VIRT_LINK = 1u << 6,
  VIRT_RAW = 0u << 6,
  VIRT_VIRTUAL = 2u << 6,
  VIRT_FILE = 3u << 6,

  HTML_TYPE = 1u << 4,
  JS_TYPE = 2u << 4,
  CSS_TYPE = 3u << 4,

  PARTIAL_RENDER = 1u << 2,
  FULL_RENDER = 2u << 2,

  NO_TEMPLATE = 0u << 1,
  FULL_TEMPLATE = 1u << 1,

  UNLOCKED = 0u,
  LOCKED = 1u,

};

// 0 - outgoing dependency
// 1 - incomming dependency
enum { DEPENDENCY_OUT = 0, DEPENDENCY_IN = 1 };

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

  int add_virtual_template(std::string name, const std::string parent_name,
                           const bool parse = true);

  int add_const_template(const std::string name, const std::string data,
                         const bool parse = true,
                         const bool future_declare = false);

  void remove_template(const std::string name);

  htmlTemplate *get_template(const std::string name);

  bool exists(const std::string name);
};

using htmlTP_handle = std::unique_ptr<htmlTP_state>;

htmlTP_handle get_htmlTP_handle();

}; // namespace htmlTP
