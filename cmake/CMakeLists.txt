cmake_minimum_required(VERSION 3.28)
project(cljonic)

set(CMAKE_CXX_STANDARD 23)

include_directories(
    ../code/source
    ../code/test
    ../resources)

file(
    GLOB_RECURSE
    TESTS
    "../code/test/*.cpp")

add_compile_options(-Wfatal-errors)

add_executable(
    cljonic
    ${TESTS})


