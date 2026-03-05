#pragma once
#include "htmlTP/htmlTP.hpp"
#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace htmlTP {

struct htmlTemplate {
private:
  // TODO: move data to a TP_data struct
  std::string parent = "";
  size_t render_size_;
  size_t template_size_;
  // | 0x00 | type(2) | render_state(2) | template_state
  // | render_lock
  //  render_size is 2^24 bits
  uint32_t flags = 0b0;

  struct Buffer {
    std::unique_ptr<char[]> data = nullptr;
    size_t size = 0;
  };

  Buffer render;
  Buffer tp;

public:
  ~htmlTemplate() {
    free_render();
    free_tp();
  }
  uint virtual_state();
  void set_virtual_state(uint virtual_state_);

  bool render_lock();
  void set_render_lock(bool lock);

  bool template_state();
  void set_template_state(bool template_state);

  uint render_state();
  void set_render_state(uint render_state);

  uint type();
  void set_type(uint type);

  uint32_t render_size();
  void set_render_size(size_t size);

  uint32_t template_size();
  void set_template_size(size_t size);

  void set_parent_name(std::string name);
  std::string parent_name();

  char *tp_handle();

  char *render_handle();

  char *alloc_tp();
  char *alloc_render();

  void free_tp();
  void free_render();
};

struct Registry {

private:
  std::unordered_map<std::string, int> id_map_;
  std::unordered_map<int, std::array<std::vector<int>, 2>> dependency_map_;
  std::unordered_map<int, std::unique_ptr<htmlTemplate>> map_;

public:
  ~Registry() noexcept = default;
  int get_id(std::string name);

  int new_object(std::string name);

  htmlTemplate *get_handle(const int id_);
  htmlTemplate *get_handle(std::string name);

  bool exists(const int &_id);
  bool exists(std::string name);

  // TODO: add dependency_map interface
};

using TP_handle = std::unique_ptr<htmlTemplate>;

TP_handle new_TP_handle();

// TODO: add a function set_data() and/or get_data_handle that takes in TP_info
// struct
struct Parser {
private:
  Registry *registry;

public:
  Parser(Registry *registry_);

  void read_TP(std::string name, const std::string data = "",
               const bool re_parse = true);
  void parse_TP(std::string name, const bool force = false);
};

}; // namespace htmlTP
