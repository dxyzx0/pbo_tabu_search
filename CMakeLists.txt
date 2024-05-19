cmake_minimum_required(VERSION 3.28)
project(PBO_Heuristics)

set(CMAKE_CXX_STANDARD 14)

# add -DuseGMP to compile with GMP
# add -DuseGMPXX to compile with GMPXX
add_executable(parser parser/SimpleParser.cc)
target_link_libraries(parser gmp gmpxx)
target_compile_definitions(parser PRIVATE useGMP)
