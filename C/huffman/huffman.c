#include "huffman.h"
#include "huffman_tree.h"
#include "../decompression/bytes_vector/bytes_vector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SUCCESS 0
#define MEMORY_ERROR -1

int huffman_compression_init(huffman_compression_t *huffman) {
    if (huffman_tree_init(&huffman->tree) != SUCCESS) {
        return MEMORY_ERROR;
    }
    return SUCCESS;
}

char huffman_compression_decode(const huffman_compression_t* self, const char* tree_code) {
    return huffman_tree_get_letter(&self->tree, tree_code);
}

bytes_vector_t* huffman_compression_encode(const huffman_compression_t* self, char letter) {
    bytes_vector_t* reverse_tree_code = huffman_tree_get_code(&self->tree, letter);
    const char* code_ptr = bytes_vector_get_ptr(reverse_tree_code);
    bytes_vector_t* tree_code = calloc(1, sizeof(bytes_vector_t));
    if (!tree_code || bytes_vector_init(tree_code)) {
        fprintf(stderr, "Could not allocate memory for the tree code!");
        exit(1);
    }
    for (unsigned long i = strlen(code_ptr); i >= 0; --i) {
        bytes_vector_add_byte(tree_code, *(code_ptr + i));
    }
    bytes_vector_release(reverse_tree_code);
    return tree_code;
}

void huffman_compression_release(huffman_compression_t *huffman) {
    huffman_tree_release(&huffman->tree);
}

void huffman_compression_print_tree(huffman_compression_t *huffman) {
    huffman_tree_print(&huffman->tree);
}