cmake_minimum_required(VERSION 3.17)
project(C C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(HuffmanCompression main.c huffman/huffman_node.h huffman/huffman_node.c huffman/nodes_list.c huffman/nodes_list.h huffman/huffman_tree.c huffman/huffman_tree.h huffman/huffman.c huffman/huffman.h)