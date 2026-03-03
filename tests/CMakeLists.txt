add_executable(temp_test test.cpp)
target_link_libraries(temp_test PRIVATE htmlTP)
add_test(NAME temp_test COMMAND $<TARGET_FILE: temp_test>)
