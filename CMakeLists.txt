cmake_minimum_required (VERSION 3.8)

project ("linkedlist2024")

add_executable (linkedlist2024 "linkedlist2024.cpp" "linkedlist2024.h")

include(CTest)
add_test(NAME test_linked_lists COMMAND linkedlist2024)
