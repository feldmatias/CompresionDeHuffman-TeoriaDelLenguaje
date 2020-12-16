cmake_minimum_required(VERSION 3.16)
#project(C)
project(C C)

#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 11)

#include_directories(.)
#include_directories(huffman)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

#add_executable(C huffman/huffman.c huffman/huffman.h huffman/huffman_node.c huffman/huffman_node.h huffman/huffman_tree.c huffman/huffman_tree.h huffman/nodes_list.c huffman/nodes_list.h CMakeLists.txt main.c)
add_executable(HuffmanCompression main.c huffman/huffman_node.h huffman/huffman_node.c
               huffman/nodes_list.c huffman/nodes_list.h huffman/huffman_tree.c
               huffman/huffman_tree.h huffman/huffman.c huffman/huffman.h
               decompression/decompression.c decompression/decompression.h decompression/bytes_vector/bytes_vector.c decompression/bytes_vector/bytes_vector.h)