#pragma once
namespace htmlTP {
constexpr char JS_EXTENSIONS[3][8] = {".js", ".jcs", "jms"};
constexpr char HTML_EXTENSIONS[3][8] = {".html"};
constexpr char CSS_EXTENSIONS[3][8] = {".css"};

constexpr char START_CLAUSE[2] = {'{', '!'};
constexpr char END_CLAUSE[2] = {'!', '}'};
enum { CLAUSE_LENGTH = 2 };

}; // namespace htmlTP
