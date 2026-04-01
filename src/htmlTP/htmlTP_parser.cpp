#include "htmlTP_priv.hpp"
#include "htmlTP_utils.hpp"
#include <cstring>
#include <fstream>
#include <stdexcept>

namespace htmlTP {

Parser::Parser(Registry *registry_) { registry = registry_; }

void Parser::parse_TP(std::string name, bool force) {
  // TODO: write a parsing function
  // TODO: add compilation commands parsing of template
  // TODO: think of a way to put a watchdog on templates and renders to
  // recompile/reread/reparse after change?
}

void Parser::read_TP(std::string name, const std::string data,
                     const bool re_parse) {
  // if reparse flag is set parsing is done auto:
  if (re_parse) {
    parse_TP(name, registry);
  }

  htmlTemplate *TP = registry->get_handle(name);

  if (TP->virtual_state() == VIRT_LINK && TP->parent_name() != "" &&
      registry != nullptr) {
    if (!registry->exists(TP->parent_name())) {
      throw std::runtime_error("No defined template reference " +
                               TP->parent_name());
    }
    TP->link_tp_buf(registry->get_handle(TP->parent_name())->get_render_link());
  }
  // Template read out of parent render
  if (TP->virtual_state() == VIRT_VIRTUAL && TP->parent_name() != "" &&
      registry != nullptr) {
    if (!registry->exists(TP->parent_name())) {
      throw std::runtime_error("No defined template reference " +
                               TP->parent_name());
    }

    *TP->alloc_tp() = *registry->get_handle(TP->parent_name())->render_handle();
    return;
  }

  // Template read out of file
  else if (TP->virtual_state() == VIRT_FILE && TP->parent_name() != "") {
    if (!file_exists(TP->parent_name())) {
      throw std::runtime_error("File " + TP->parent_name() + " not found");
    }
    std::fstream tp_file(TP->parent_name());
    tp_file.read(TP->alloc_tp(), TP->template_size());
    tp_file.close();
    return;
  }

  // Template read out of char array
  else if (TP->virtual_state() == VIRT_RAW && data != "") {
    TP->set_template_size(data.length());
    *TP->alloc_tp() = *data.c_str();
    return;
  }

  throw std::runtime_error("Could not resolve template source for: " + name);
}
void Parser::parse_compilation_commands(Compilation_commands *comp_commands,
                                        Buffer *buffer,
                                        bool future_declare = false) {

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
            "Template definition clauses missmached, missing start clause");
      }
      comp_commands->back()[1] = (int)(current_position - buffer->data.get()) -
                                 comp_commands->back()[0] + CLAUSE_LENGTH;
      // TODO: replace id with hash of name for future declarations
      std::string tp_name =
          substr(buffer->data.get(), comp_commands->back()[0] + CLAUSE_LENGTH,
                 comp_commands->back()[1] - CLAUSE_LENGTH * 2);

      // If name exists add id, if not and future declaration exists, generate
      // id, else throw error
      if (!registry->exists(tp_name)) {
        if (future_declare) {
          comp_commands->back()[2] = id_gen(tp_name);
        } else {
          throw std::runtime_error("Future declaration not permited");
        }
      } else {
        comp_commands->back()[2] = registry->get_id(tp_name);
      }
    }
  }

  if (comp_commands->back()[1] == 0) {
    throw std::runtime_error(
        "Template definition clauses missmached, missing end clause");
  }
}
}; // namespace htmlTP
