#include "htmlTP_priv.hpp"
#include "utils.hpp"

namespace htmlTP {
int Registry::get_id(std::string name) {
  if (!exists(name)) {
    throw std::range_error("Value " + name + " not in registry");
  }

  clear_name(name);
  return id_map_[name];
}

int Registry::new_object(std::string name) {
  if (exists(name)) {
    throw std::runtime_error("Name " + name + "already in registry");
  }
  clear_name(name);

  // unique ID
  int id_;
  do {
    id_ = id_gen();
  } while (exists(id_));

  // enter into name and object registry
  id_map_[name] = id_;
  map_[id_] = new_TP_handle();
  return id_;
}

htmlTemplate &Registry::get_handle(const int id_) {
  if (!exists(id_)) {
    throw std::range_error("Value " + std::to_string(id_) + " not in registry");
  }
  return *map_[id_].get();
}

htmlTemplate &Registry::get_handle(std::string name) {
  return get_handle(get_id(name));
}

bool Registry::exists(const int &_id) {
  return (map_.find(_id) == map_.end()) ? false : true;
}

bool Registry::exists(std::string name) {
  clear_name(name);
  return (id_map_.find(name) == id_map_.end()) ? false : true;
}
}; // namespace htmlTP
