#ifndef C_HUFFMAN_H
#define C_HUFFMAN_H

#include "huffman_tree.h"

//struct huffman_compression;
//typedef struct huffman_compression huffman_compression_t;

typedef struct huffman_compression {
    huffman_tree_t tree;
}huffman_compression_t;

//huffman_compression_t *huffman_compression_create();

//void huffman_compression_destroy(huffman_compression_t *huffman);

int huffman_compression_init(huffman_compression_t *huffman);

void huffman_compression_release(huffman_compression_t *huffman);

void huffman_compression_print_tree(huffman_compression_t *huffman);

#endif //C_HUFFMAN_H
