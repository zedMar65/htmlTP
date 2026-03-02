#include "extensions.hpp"
#include "htmlTP/htmlTP.hpp"
#include <algorithm>
#include <array>
#include <filesystem>
#include <memory>

void htmlTP::htmlTP_state::add_template(const std::string *name,
                                        const std::string *file, uint *type) {
  if (exists(name)) {
    throw std::runtime_error("Duplicate key: " + *name);
  }
  registry[*name] = std::make_unique<htmlTP::htmlTemplate>();
  htmlTP::htmlTemplate *tp = registry[*name].get();
  tp->set_template_size(std::filesystem::file_size(*file));
  *tp->name_handle() = *name;
  *tp->file_handle() = *file;
  std::filesystem::path p{*file};

  // Find type if not given
  if (type != nullptr) {
  } else {
    std::filesystem::path extension = p.extension();
    if (std::find(std::begin(htmlTP::JS_EXTENSIONS),
                  std::end(htmlTP::JS_EXTENSIONS),
                  extension) != std::end(htmlTP::JS_EXTENSIONS)) {
      *type = htmlTP::JS_TYPE;
    } else if (std::find(std::begin(htmlTP::HTML_EXTENSIONS),
                         std::end(htmlTP::HTML_EXTENSIONS),
                         extension) != std::end(htmlTP::HTML_EXTENSIONS)) {
      *type = htmlTP::HTML_TYPE;
    } else if (std::find(std::begin(htmlTP::CSS_EXTENSIONS),
                         std::end(htmlTP::CSS_EXTENSIONS),
                         extension) != std::end(htmlTP::CSS_EXTENSIONS)) {
      *type = htmlTP::CSS_TYPE;
    }
  }

  tp->set_type(*type);
}

void htmlTP::htmlTP_state::add_virtual_template(const std::string *name,
                                                const char *render,
                                                const uint render_size,
                                                const uint *type) {

  if (exists(name)) {
    throw std::runtime_error("Duplicate key: " + *name);
  }
  registry[*name] = std::make_unique<htmlTP::htmlTemplate>();
  htmlTP::htmlTemplate *tp = registry[*name].get();
  tp->set_template_size(render_size);
  *tp->tp_handle() = std::make_unique<char[]>(render_size);
  *((*tp->tp_handle()).get()) = *render;

  tp->set_type(*type);
}

void htmlTP::htmlTP_state::remove_template(const std::string *name) {
  if (!exists(name)) {
    throw std::out_of_range("Key not found: " + *name);
  }
  registry.erase(*name);
}

bool htmlTP::htmlTP_state::exists(const std::string *name) {
  return (registry.find(*name) == registry.end()) ? false : true;
}

htmlTP::htmlTemplate *get_template(const std::string *name);
