cmake_minimum_required(VERSION 3.10.0)
project(april VERSION 0.1.0 LANGUAGES C)

enable_testing()

add_subdirectory(test)

add_library(april
    header.h
    fwd.h
    mem.h mem.c
    graph.h graph.c
    context.h context.c
    g_database.h g_database.c
    g_table.h g_table.c
    april.h april.c)

