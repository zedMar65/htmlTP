#pragma once
#include "htmlTP/htmlTP.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace htmlTP {

constexpr int makefourcc(char c1, char c2, char c3 = 0, char c4 = 0) {
  return (static_cast<int>(c1)) | (static_cast<int>(c2) << 8) |
         (static_cast<int>(c3) << 16) | (static_cast<int>(c4) << 24);
}

enum {
  START_CLAUSE = makefourcc('{', '!'),
  END_CLAUSE = makefourcc('!', '}'),
  CLAUSE_LENGTH = 2
};

struct TP_data {
  std::string parent = "";
  size_t render_size_ = 0;
  size_t template_size_ = 0;
  int render_hash_ = 0;
  int template_hash_ = 0;
  int compilation_hash = 0;
  uint32_t flags = 0b0;
};

typedef std::vector<std::array<int, 2>> Compilation_commands;

struct Buffer {
  std::unique_ptr<char[]> data = nullptr;
  // size defined in buffer is current allocation size
  size_t size = 0;
};
struct htmlTemplate {
private:
  TP_data data;

  // NOT proud of this, but it miiiiiight? work with propper cleanups?
  Buffer tp_stack = Buffer();
  Buffer render_stack = Buffer();

  Buffer *tp__ = &tp_stack;
  Buffer *render__ = &render_stack;

  std::vector<std::array<int, 2>> compilation_commands;

public:
  ~htmlTemplate() {
    free_render();
    free_tp();
  }

  void set_render_hash();
  void set_template_hash();

  Compilation_commands *compilation_commands_handle();
  void set_compilaion_hash();

  int *render_hash_handle();
  int *template_hash_handle();

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

  void link_tp_buf(Buffer *buf);
  void unlink_tp_buf();

  Buffer *get_render_link();
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

  void parse_compilation(std::string name);
  void read_TP(std::string name, const std::string data = "",
               const bool re_parse = true);
  void parse_TP(std::string name, const bool force = false);
};

}; // namespace htmlTP
