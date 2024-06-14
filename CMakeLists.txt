cmake_minimum_required(VERSION 3.25)
project(PBO_Heuristics)

set(CMAKE_CXX_STANDARD 23)

# add -DuseGMP to compile with GMP
# add -DuseGMPXX to compile with GMPXX
add_executable(parser parser/SimpleParser.cc
        parser/SimpleParser.h
        parser/DefaultCallback.cpp
        parser/DefaultCallback.h
        parser/PboCallback.cpp
        parser/PboCallback.h)
target_link_libraries(parser gmp gmpxx)
target_compile_definitions(parser PRIVATE useGMP)

option(WITH_EIGEN "Use Eigen" ON)
option(WITH_GMP "Use GMP" OFF)
option(WITH_CONDA "Use Conda" ON)

if (WITH_CONDA)
    list(APPEND CMAKE_PREFIX_PATH "/home/psw/mambaforge/envs/eigen")
    message(STATUS "CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")
endif ()

if (WITH_EIGEN)
    set(COMPILE_DEFS ${COMPILE_DEFS} useEigen)
    find_package(Eigen3 REQUIRED)
    set(LIBRARYS ${LIBRARYS} Eigen3::Eigen)
endif ()

if (WITH_GMP)
    set(COMPILE_DEFS ${COMPILE_DEFS} useGMP)
    set(LIBRARYS ${LIBRARYS} gmp gmpxx)
endif ()

find_package(spdlog CONFIG REQUIRED)
set(LIBRARYS ${LIBRARYS} spdlog::spdlog)

add_executable(pbo_heur_eigen
        main.cpp
        parser/DefaultCallback.cpp
        parser/PboCallback.cpp
        utils.cpp
        Messenger.cpp
        Problem.cpp
        Settings.cpp
        Solver.cpp
        Presolver.cpp
        Propgators.cpp
        Heuristics.cpp
        PresolverNaive.cpp
        HeuristicsRandom.cpp
        HeuristicsTabuSearch.cpp
)
target_link_libraries(pbo_heur_eigen ${LIBRARYS})
target_compile_definitions(pbo_heur_eigen PRIVATE ${COMPILE_DEFS})
