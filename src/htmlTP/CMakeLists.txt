target_include_directories(htmlTP PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(TESTING)

  target_compile_options(htmlTP PUBLIC -Wall -Wextra -g)

target_sources(
  htmlTP
  PUBLIC htmlTP_state.cpp ${PROJECT_SOURCE_DIR}/include/htmlTP/htmlTP.hpp
  htmlTP_template.cpp
          htmlTP_priv.hpp
          htmlTP_extensions.hpp
          htmlTP_utils.cpp
          htmlTP_utils.hpp
          htmlTP_registry.cpp
          htmlTP_parser.cpp)
else()

target_sources(
  htmlTP
  PRIVATE htmlTP_state.cpp ${PROJECT_SOURCE_DIR}/include/htmlTP/htmlTP.hpp
  htmlTP_template.cpp
          htmlTP_priv.hpp
          htmlTP_extensions.hpp
          htmlTP_utils.cpp
          htmlTP_utils.hpp
          htmlTP_registry.cpp
          htmlTP_parser.cpp)
endif()
