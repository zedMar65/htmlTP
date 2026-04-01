#include "htmlTP_utils.hpp"
#include "htmlTP/htmlTP.hpp"
#include "htmlTP_priv.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <random>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
namespace htmlTP {
IsChars::IsChars(const char *charsToRemove) : chars(charsToRemove) {};

bool IsChars::operator()(char c) {
  for (const char *testChar = chars; *testChar != 0; ++testChar) {
    if (*testChar == c) {
      return true;
    }
  }
  return false;
}

bool file_exists(std::string file_name) {

  struct stat sb;

  if (stat(file_name.c_str(), &sb) != 0) {
    return false;
  }
  return true;
}

int id_gen() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist;

  return dist(gen);
}

void clear_name(std::string &name) {
  name.erase(std::remove_if(name.begin(), name.end(), IsChars("<\"\">")),
             name.end());
}

int parse_virtual_by_name(std::string parent_) {
  if (parent_.length() < 2) {
    return ERROR;
  }
  // TODO: replace with START_CLAUSE and END_CLAUSE
  if (parent_[0] == '(' && parent_.back() == ')') {
    if (parent_[1] == '*') {
      return VIRT_LINK;
    }
    return VIRT_VIRTUAL;
  }
  if (file_exists(parent_)) {
    return VIRT_FILE;
  }
  return VIRT_RAW;
}

std::string clause_to_string(int a, int len) {
  std::string b(len, ' ');
  for (int i = 0; i < len; i++) {
    b[i] = char((a >> i * 8) & 0xFFu);
  }
  return b;
}

std::string substr(char *arr, int begin, int len) {
  char *res = new char[len + 1];
  for (int i = 0; i < len; i++)
    res[i] = *(arr + begin + i);
  res[len] = 0;
  return res;
}
void parse_compilation_commands(Compilation_commands *comp_commands,
                                Buffer *buffer, Registry *reg) {

  *comp_commands = Compilation_commands();

  const char *end_position = buffer->data.get() + buffer->size;

  const std::string start_key = clause_to_string(START_CLAUSE, CLAUSE_LENGTH);
  const std::string end_key = clause_to_string(END_CLAUSE, CLAUSE_LENGTH);

  for (char *current_position = buffer->data.get();
       current_position < end_position - CLAUSE_LENGTH + 1;
       current_position += 1) {
    if (memcmp(current_position, start_key.c_str(), CLAUSE_LENGTH) == 0) {
      comp_commands->push_back(
          {(int)(current_position - buffer->data.get()), 0, 0});
    }
    if (memcmp(current_position, end_key.c_str(), CLAUSE_LENGTH) == 0) {
      if (comp_commands->back()[1] != 0) {
        throw std::runtime_error(
            "Template definition clauses missmached, !} before {!");
      }
      comp_commands->back()[1] = (int)(current_position - buffer->data.get()) -
                                 comp_commands->back()[0] + CLAUSE_LENGTH;
      comp_commands->back()[2] = reg->get_id(
          substr(buffer->data.get(), comp_commands->back()[0] + CLAUSE_LENGTH,
                 comp_commands->back()[1] - CLAUSE_LENGTH * 2));
    }
  }
}

size_t vector22_hash(std::vector<std::array<int, 2>> *v) {
  size_t hash = 0;
  for (int i = 0; i < v->size(); i++) {
    hash = (hash + (324723947 + (*v)[i][0])) ^ 93485734985;
    hash = (hash + (324723947 + (*v)[i][1])) ^ 93485734985;
  }
  return hash;
}

}; // namespace htmlTP
