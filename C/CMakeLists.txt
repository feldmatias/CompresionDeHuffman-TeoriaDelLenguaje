cmake_minimum_required(VERSION 3.16)
project(HUFFMAN)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(HuffmanCompression main.c huffman/huffman_node.h huffman/huffman_node.c
               huffman/nodes_list.c huffman/nodes_list.h huffman/huffman_tree.c
               huffman/huffman_tree.h huffman/huffman.c huffman/huffman.h
               decompression/decompression.c decompression/decompression.h
               decompression/bytes_vector/bytes_vector.c
               decompression/bytes_vector/bytes_vector.h
               compression/compression.c compression/compression.h)

target_compile_options(HuffmanCompression PRIVATE -O3 -ggdb)

target_link_libraries(HuffmanCompression PRIVATE -lm)