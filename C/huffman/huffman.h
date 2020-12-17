#ifndef C_HUFFMAN_H
#define C_HUFFMAN_H

#include "huffman_tree.h"

typedef struct huffman_compression {
    huffman_tree_t tree;
}huffman_compression_t;

//Returns 0 if successful, otherwise returns -1
int huffman_compression_init(huffman_compression_t *huffman);

void huffman_compression_release(huffman_compression_t *huffman);

void huffman_compression_print_tree(huffman_compression_t *huffman);

#endif //C_HUFFMAN_H
