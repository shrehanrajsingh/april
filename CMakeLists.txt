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
    g_field.h g_field.c
    g_entry.h g_entry.c
    g_tuple.h g_tuple.c
    ht.h ht.c
    urand.h urand.c
    april.h april.c)

