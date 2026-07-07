#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include "htmlTP_utils.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace htmlTP {

int Registry::get_id(std::string name) {
  if (!exists(name)) {
    throw std::range_error("Value " + name + " not in registry");
  }
  if (isInteger(name)) {
    if (exists(std::stoi(name))) {
      return std::stoi(name);
    }
  }
  name = clear_name(name);
  return id_map_[name];
}

int Registry::new_object(std::string name) {
  if (exists(name)) {
    throw std::runtime_error("Name " + name + "already in registry");
  }
  name = clear_name(name);

  // unique ID
  int id_;
  id_ = id_gen(name);
  if (exists(id_)) {
    throw std::runtime_error("Internal error id already exists");
  }

  // enter into name and object registry
  id_map_[name] = id_;
  map_[id_] = new_TP_handle();

  // add dependency map instance
  dependency_map_[id_] = Dep_vector();
  return id_;
}

htmlTemplate *Registry::get_handle(const int id_) {
  if (!exists(id_)) {
    throw std::range_error("Value " + std::to_string(id_) + " not in registry");
  }
  return map_[id_].get();
}

htmlTemplate *Registry::get_handle(std::string name) {
  if (!exists(name)) {
    throw std::range_error("Value " + name + " not in registry");
  }
  return get_handle(get_id(name));
}

bool Registry::exists(const int id_) {
  return (map_.find(id_) == map_.end()) ? false : true;
}

bool Registry::exists(std::string name) {
  name = clear_name(name);
  return (id_map_.find(name) == id_map_.end()) ? false : true;
}

void Registry::add_dependency(std::string name, std::string name_dep,
                              bool dependency_direction) {
  if (!exists(name)) {
    throw std::range_error("Value " + name + " not in registry");
  }
  if (!exists(name_dep)) {
    throw std::range_error("Value " + name_dep + " not in registry");
  }
  add_dependency(get_id(name), get_id(name_dep), dependency_direction);
}

void Registry::add_dependency(const int id_, const int id_dep,
                              bool dependency_direction) {

  if (!exists(id_)) {
    throw std::range_error("Value " + std::to_string(id_) + " not in registry");
  }

  if (!exists(id_dep)) {
    throw std::range_error("Value " + std::to_string(id_dep) +
                           " not in registry");
  }

  // INFO: Dependeny looping is not protected currently

  std::vector<int> *current_dependency;
  if (dependency_direction) {
    current_dependency = &dependency_map_[id_].in;
  } else {

    current_dependency = &dependency_map_[id_].out;
  }
  if (std::find((*current_dependency).begin(), (*current_dependency).end(),
                id_dep) != (*current_dependency).end()) {
    // Already dependant
    return;
  }
  (*current_dependency).push_back(id_dep);
  return;
}

Dep_vector *Registry::get_dependency(std::string name) {

  if (!exists(name)) {
    throw std::range_error("Value " + name + " not in registry");
  }
  return get_dependency(get_id(name));
}
Dep_vector *Registry::get_dependency(const int id_) {
  if (!exists(id_)) {
    throw std::range_error("Value " + std::to_string(id_) + " not in registry");
  }
  return &dependency_map_[id_];
}
}; // namespace htmlTP
