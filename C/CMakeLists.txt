cmake_minimum_required(VERSION 3.16)
project(HUFFMAN)

add_executable(huffman main.c huffman/huffman_node.h huffman/huffman_node.c
               huffman/nodes_list.c huffman/nodes_list.h huffman/huffman_tree.c
               huffman/huffman_tree.h huffman/huffman.c huffman/huffman.h
               decompression/decompression.c decompression/decompression.h
               decompression/bytes_vector/bytes_vector.c
               decompression/bytes_vector/bytes_vector.h
               compression/compression.c compression/compression.h)

target_compile_options(huffman PRIVATE -O3 -ggdb)

target_link_libraries(huffman PRIVATE -lm)