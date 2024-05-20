cmake_minimum_required(VERSION 3.28)
project(PBO_Heuristics)

set(CMAKE_CXX_STANDARD 23)

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
    add_executable(pbo_heur_eigen
            main.cpp
            parser/SimpleParser.h
            parser/DefaultCallback.cpp
            parser/DefaultCallback.h
            parser/AbcCallback.cpp
            parser/AbcCallback.h
            type.h
            Messenger.cpp
            Messenger.h
            Problem.cpp
            Problem.h
            Settings.cpp
            Settings.h
            Solver.cpp
            Solver.h
            Presolver.cpp
            Presolver.h
            Propgators.cpp
            Propgators.h
            Heuristics.cpp
            Heuristics.h
            PresolverNaive.cpp
            PresolverNaive.h
            HeuristicsRandom.cpp
            HeuristicsRandom.h
    )
    target_link_libraries(pbo_heur_eigen gmp gmpxx)
    target_compile_definitions(pbo_heur_eigen PRIVATE useGMP useEigen)
endif ()
