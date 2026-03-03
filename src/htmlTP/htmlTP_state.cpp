#include "extensions.hpp"
#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <algorithm>
#include <array>
#include <filesystem>
#include <memory>
#include <sys/stat.h>

struct htmlTP::htmlTP_state::Registry {
  ~Registry() = default;
  std::unordered_map<std::string, std::unique_ptr<htmlTP::htmlTemplate>> map_;
};

htmlTP::htmlTP_state::htmlTP_state() {
  registry = std::make_unique<Registry>();
}

htmlTP::htmlTP_state::~htmlTP_state() = default;

void htmlTP::htmlTP_state::add_template(const std::string &name,
                                        const std::string &file, uint type) {
  if (exists(name)) {
    throw std::runtime_error("Duplicate key: " + name);
  }
  registry->map_[name] = htmlTP::get_TP_handle();
  htmlTP::htmlTemplate *tp = registry->map_[name].get();

  struct stat sb;

  if (stat(file.c_str(), &sb) != 0) {
    throw std::runtime_error("No file or directory: " + file);
  }
  tp->set_template_size(std::filesystem::file_size(file));
  *tp->name_handle() = name;
  *tp->file_handle() = file;
  std::filesystem::path p{file};

  // Find type if not given
  if (type != 0) {
  } else {
    std::filesystem::path extension = p.extension();
    if (std::find(std::begin(htmlTP::JS_EXTENSIONS),
                  std::end(htmlTP::JS_EXTENSIONS),
                  extension) != std::end(htmlTP::JS_EXTENSIONS)) {
      type = htmlTP::JS_TYPE;
    } else if (std::find(std::begin(htmlTP::HTML_EXTENSIONS),
                         std::end(htmlTP::HTML_EXTENSIONS),
                         extension) != std::end(htmlTP::HTML_EXTENSIONS)) {
      type = htmlTP::HTML_TYPE;
    } else if (std::find(std::begin(htmlTP::CSS_EXTENSIONS),
                         std::end(htmlTP::CSS_EXTENSIONS),
                         extension) != std::end(htmlTP::CSS_EXTENSIONS)) {
      type = htmlTP::CSS_TYPE;
    }
  }

  tp->set_type(type);
}

void htmlTP::htmlTP_state::add_virtual_template(const std::string &name,
                                                const char *render,
                                                const uint render_size,
                                                const uint &type) {

  if (exists(name)) {
    throw std::runtime_error("Duplicate key: " + name);
  }
  registry->map_[name] = htmlTP::get_TP_handle();
  htmlTP::htmlTemplate *tp = registry->map_[name].get();
  tp->set_template_size(render_size);
  *tp->tp_handle() = std::make_unique<char[]>(render_size);
  *((*tp->tp_handle()).get()) = *render;

  tp->set_type(type);
}

void htmlTP::htmlTP_state::remove_template(const std::string &name) {
  if (!exists(name)) {
    throw std::out_of_range("Key not found: " + name);
  }
  registry->map_.erase(name);
}

bool htmlTP::htmlTP_state::exists(const std::string &name) {
  return (registry->map_.find(name) == registry->map_.end()) ? false : true;
}

htmlTP::htmlTP_handle htmlTP::get_htmlTP_handle() {
  return std::make_unique<htmlTP::htmlTP_state>();
}
