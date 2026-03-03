#include "extensions.hpp"
#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <memory>
#include <sys/stat.h>
#include <unordered_map>
#include <vector>

namespace htmlTP {
struct htmlTP_state::Registry {
  ~Registry() = default;
  std::unordered_map<std::string, int> id_map_;
  std::unordered_map<int, std::vector<int>[2]> dependency_map_;
  std::unordered_map<int, std::unique_ptr<htmlTemplate>> map_;
  int &get_id(const std::string &name) { return id_map_[name]; }
};

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

void htmlTP_state::add_virtual_template(const std::string &name,
                                        const std::string &dependency_name,
                                        uint32_t template_size, uint type,
                                        char *raw_data[], bool parse,
                                        uint virtuality) {

  if (exists(name)) {
    throw std::runtime_error("Duplicate key: " + name);
  }

  // Ascosiate with name and file
  TP_handle TP = get_TP_handle();
  *TP->name_handle() = name;
  *TP->file_handle() = dependency_name;
  int &id = *TP->set_id();
  registry->map_[id] = std::move(TP);
  registry->id_map_[name] = id;
  htmlTemplate &tp = *get_template(name);

  // Parse
  if (parse) {
    parse_TP(tp);
  }

  // Force replace some data
  if (type != UNDEFINED) {
    tp.set_type(type);
  }
  if (template_size != UNDEFINED) {
    tp.set_template_size(template_size);
  }
  if (raw_data != nullptr) {
    read_TP(tp, raw_data);
  }
  if (virtuality != UNDEFINED) {
  }
}

void htmlTP_state::remove_template(const std::string &name) {
  if (!exists(name)) {
    throw std::out_of_range("Key not found: " + name);
  }
  registry->map_.erase(registry->get_id(name));
}

bool htmlTP_state::exists(const std::string &name) {
  return (registry->map_.find(registry->get_id(name)) == registry->map_.end())
             ? false
             : true;
}

htmlTemplate *htmlTP_state::get_template(const std::string &name) {
  if (!exists(name)) {
    throw std::out_of_range("Key not found: " + name);
  }
  return registry->map_[registry->get_id(name)].get();
};

htmlTP_handle get_htmlTP_handle() { return std::make_unique<htmlTP_state>(); }

}; // namespace htmlTP
