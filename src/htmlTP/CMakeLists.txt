target_include_directories(htmlTP PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(TESTING)

  target_compile_options(htmlTP PUBLIC -Wall -Wextra -g)
endif()

target_sources(
  htmlTP
  PUBLIC htmlTemplate.cpp htmlTP_state.cpp
         ${PROJECT_SOURCE_DIR}/include/htmlTP/htmlTP.hpp
  PRIVATE htmlTP_priv.hpp extensions.hpp utils.cpp utils.hpp registry.cpp)
