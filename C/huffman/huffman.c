#include <stdlib.h>
#include "huffman.h"
#include "huffman_tree.h"

#define SUCCESS 0
#define MEMORY_ERROR -1

int huffman_compression_create(huffman_compression_t *huffman) {
    if (huffman_tree_init(&huffman->tree) != SUCCESS) {
        huffman_tree_release(&huffman->tree);
        return MEMORY_ERROR;
    }
    return SUCCESS;
}

char huffman_compression_decode(const huffman_compression_t* self, const char* tree_code) {
    return huffman_tree_get_letter(self->tree, tree_code);
}

bytes_vector_t* huffman_compression_encode(const huffman_compression_t* self, char letter) {
    return huffman_tree_get_code(self->tree, letter);
}

void huffman_compression_destroy(huffman_compression_t *huffman) {
    huffman_tree_release(&huffman->tree);
}

void huffman_compression_print_tree(huffman_compression_t *huffman) {
    huffman_tree_print(&huffman->tree);
}