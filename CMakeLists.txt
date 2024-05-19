cmake_minimum_required(VERSION 3.28)
project(PBO_Heuristics)

set(CMAKE_CXX_STANDARD 14)

# add -DuseGMP to compile with GMP
# add -DuseGMPXX to compile with GMPXX
add_executable(parser parser/SimpleParser.cc
        parser/SimpleParser.h
        parser/DefaultCallback.cpp
        parser/DefaultCallback.h
        parser/AbcCallback.cpp
        parser/AbcCallback.h)
target_link_libraries(parser gmp gmpxx)
target_compile_definitions(parser PRIVATE useGMP)

# add Eigen as a dependency
option(useEigen "Use Eigen" ON)
if (useEigen)
    find_package(Eigen3 3.4 REQUIRED NO_MODULE)
    add_executable(pbo_heur_eigen
            main.cpp
            Problem.cpp
            Problem.h
            type.h
            Presolver.cpp
            Presolver.h
    )
    target_link_libraries(pbo_heur_eigen gmp gmpxx Eigen3::Eigen)
    target_compile_definitions(pbo_heur_eigen PRIVATE useGMP useEigen)
endif ()
