#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/stat.h>

namespace htmlTP {

htmlTP_state::htmlTP_state() { registry = std::make_unique<Registry>(); }

htmlTP_state::~htmlTP_state() = default;

// void htmlTP_state::add_template(const std::string &name,
//                                 const std::string &file, uint type,
//                                 uint32_t template_size) {
//   if (exists(name)) {
//     throw std::runtime_error("Duplicate key: " + name);
//   }
//   registry->map_[name] = get_TP_handle();
//   htmlTemplate *tp = registry->map_[name].get();
//
//   struct stat sb;
//
//   if (stat(file.c_str(), &sb) != 0) {
//     throw std::runtime_error("No file or directory: " + file);
//   }
//   *tp->name_handle() = name;
//   tp->set_id();
//   *tp->file_handle() = file;
//   std::filesystem::path p{file};
//
//   // Find type if not given
//   if (type != 0) {
//   } else {
//     std::filesystem::path extension = p.extension();
//     if (std::find(std::begin(JS_EXTENSIONS), std::end(JS_EXTENSIONS),
//                   extension) != std::end(JS_EXTENSIONS)) {
//       type = JS_TYPE;
//     } else if (std::find(std::begin(HTML_EXTENSIONS),
//     std::end(HTML_EXTENSIONS),
//                          extension) != std::end(HTML_EXTENSIONS)) {
//       type = HTML_TYPE;
//     } else if (std::find(std::begin(CSS_EXTENSIONS),
//     std::end(CSS_EXTENSIONS),
//                          extension) != std::end(CSS_EXTENSIONS)) {
//       type = CSS_TYPE;
//     }
//   }
//
//   tp->set_type(type);
// }

int htmlTP_state::add_virtual_template(std::string name,
                                       std::string parent_name,
                                       uint32_t template_size, uint type,
                                       char *raw_data[], bool parse,
                                       uint virtuality) {

  if (exists(name)) {
    throw std::runtime_error("Duplicate key: " + name);
  }

  int id = registry->new_object(name);
  htmlTemplate &tp = registry->get_handle(id);

  // Parse

  // Force replace some data
  if (type != UNDEFINED) {
    tp.set_type(type);
  }
  if (template_size != UNDEFINED) {
    tp.set_template_size(template_size);
  }
  if (virtuality != UNDEFINED) {
    tp.set_virtual_state(virtuality);
  }
  if (parent_name != "") {
    tp.set_parent_name(parent_name);
  }
  if (raw_data != nullptr) {
    // TODO: read_TP(tp, raw_data);
  }

  if (parse) {
    parse_TP(tp);
  }
  return id;
}

htmlTP_handle get_htmlTP_handle() { return std::make_unique<htmlTP_state>(); }

}; // namespace htmlTP
