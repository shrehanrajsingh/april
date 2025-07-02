cmake_minimum_required(VERSION 3.10.0)
project(april VERSION 0.1.0 LANGUAGES C)

enable_testing()

add_subdirectory(test)

add_library(april
    header.h
    mem.h mem.c
    graph.h graph.c
    april.h april.c)

