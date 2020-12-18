#ifndef C_HUFFMAN_H
#define C_HUFFMAN_H

#include "huffman_tree.h"

typedef struct bytes_vector bytes_vector_t;

typedef struct huffman_compression {
    huffman_tree_t tree;
} huffman_compression_t;

//Returns 0 if successful, otherwise returns -1
int huffman_compression_init(huffman_compression_t *huffman);

huffman_compression_t *huffman_compression_create();

/*
 * Given a letter code, returns the corresponded letter. If the code is not associated
 * with a letter then it returns -1.
 */
char huffman_compression_decode(const huffman_compression_t* self, const char* tree_code);

/*
 * Given a letter, returns the corresponded code. If the letter does not exist it returns
 * an empty string.
 */
bytes_vector_t* huffman_compression_encode(const huffman_compression_t* self, char letter);

void huffman_compression_release(huffman_compression_t *huffman);

void huffman_compression_print_tree(huffman_compression_t *huffman);

#endif //C_HUFFMAN_H
