#ifndef C_HUFFMAN_H
#define C_HUFFMAN_H

struct huffman_compression;

typedef struct huffman_compression huffman_compression_t;


huffman_compression_t *huffman_compression_create();

void huffman_compression_destroy(huffman_compression_t *huffman);

void huffman_compression_print_tree(huffman_compression_t *huffman);

#endif //C_HUFFMAN_H
