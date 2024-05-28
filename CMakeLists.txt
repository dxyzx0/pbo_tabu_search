cmake_minimum_required(VERSION 3.25)
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
option(useCONDA "Use Conda" ON)
if (useCONDA)
    list(APPEND CMAKE_PREFIX_PATH "/home/optimi/spu/mambaforge/envs/cpp")
    message(STATUS "CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")
endif ()
option(useEigen "Use Eigen" ON)
if (useEigen)
    find_package(spdlog CONFIG REQUIRED)
    add_executable(pbo_heur_eigen
            main.cpp
            parser/SimpleParser.h
            parser/DefaultCallback.cpp
            parser/DefaultCallback.h
            parser/AbcCallback.cpp
            parser/AbcCallback.h
            type.h
            utils.cpp
            utils.h
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
            HeuristicsTabuSearch.cpp
            HeuristicsTabuSearch.h
    )
    target_link_libraries(pbo_heur_eigen gmp gmpxx spdlog::spdlog)
    target_compile_definitions(pbo_heur_eigen PRIVATE useGMP useEigen)
endif ()
