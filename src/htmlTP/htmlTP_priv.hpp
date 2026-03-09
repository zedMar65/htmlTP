#pragma once
#include "htmlTP/htmlTP.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace htmlTP {

struct TP_data {
  std::string parent = "";
  uint32_t render_size_ = 0;
  uint32_t template_size_ = 0;
  uint32_t flags = 0b0;
};

struct htmlTemplate {
private:
  TP_data data;
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

  void set_data(TP_data *data_);
  TP_data *get_data();

  uint virtual_state();
  void set_virtual_state(uint virtual_state_);

  uint lock();
  void set_lock(uint lock);

  bool template_lock();
  void set_template_lock(bool lock);

  uint template_state();
  void set_template_state(uint template_state);

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

struct Dep_vector {
  std::vector<int> in;
  std::vector<int> out;
};
struct Registry {
private:
  std::unordered_map<std::string, int> id_map_;
  std::unordered_map<int, Dep_vector> dependency_map_;
  std::unordered_map<int, std::unique_ptr<htmlTemplate>> map_;

public:
  ~Registry() noexcept = default;
  int get_id(std::string name);

  int new_object(std::string name);

  htmlTemplate *get_handle(const int id_);
  htmlTemplate *get_handle(std::string name);

  bool exists(const int id_);
  bool exists(std::string name);

  void add_dependency(std::string name, std::string name_dep,
                      bool dependeny_direction);
  void add_dependency(const int id_, const int id_dep,
                      bool dependeny_direction);
  Dep_vector *get_dependency(std::string name);
  Dep_vector *get_dependency(const int id_);
};

using TP_handle = std::unique_ptr<htmlTemplate>;

TP_handle new_TP_handle();

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
