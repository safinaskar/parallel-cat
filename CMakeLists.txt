cmake_minimum_required (VERSION 3.1) # 3.1 adds C_STANDARD
project (parallel-cat LANGUAGES C)
add_executable (parallel-cat parallel-cat.c)
set_property (TARGET parallel-cat PROPERTY C_STANDARD 99)
install (TARGETS parallel-cat DESTINATION bin)
