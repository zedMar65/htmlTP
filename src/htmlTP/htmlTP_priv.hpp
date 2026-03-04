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
  std::string parent = "";
  uint render_size_;
  uint template_size_;
  // | 0x00 | type(2) | render_state(2) | template_state
  // | render_lock
  //  render_size is 2^24 bits
  uint32_t flags = 0b0;
  // Compilation output
  std::unique_ptr<char[]> render = nullptr;
  // Compilation instructions
  std::unique_ptr<char[]> tp = nullptr;

public:
  ~htmlTemplate() noexcept = default;
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
  void set_render_size(uint32_t size);

  uint32_t template_size();
  void set_template_size(uint32_t size);

  void set_parent_name(std::string name);
  std::string parent_name();

  std::unique_ptr<char[]> *tp_handle();
  std::unique_ptr<char[]> *render_handle();
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

  htmlTemplate &get_handle(const int id_);
  htmlTemplate &get_handle(std::string name);

  bool exists(const int &_id);
  bool exists(std::string name);

  // TODO: add dependency_map interface
};

using TP_handle = std::unique_ptr<htmlTemplate>;

TP_handle new_TP_handle();

void parse_TP(htmlTemplate &TP, bool force, htmlTP_state *parent);
void read_TP(htmlTemplate &TP, char *data = nullptr,
             htmlTP_state *parent = nullptr);
}; // namespace htmlTP
