#include <stdlib.h>
#include "huffman.h"
#include "huffman_tree.h"

struct huffman_compression {
    huffman_tree_t *tree;
};

huffman_compression_t *huffman_compression_create() {
    huffman_compression_t *huffman = malloc(sizeof(huffman_compression_t));
    if (huffman == NULL) {
        return NULL;
    }

    huffman_tree_t *tree = huffman_tree_create();
    if (tree == NULL) {
        free(huffman);
        return NULL;
    }

    huffman->tree = tree;
    return huffman;
}

char huffman_compression_decode(const huffman_compression_t* self, const char* tree_code) {
    return huffman_tree_get_letter(self->tree, tree_code);
}

bytes_vector_t* huffman_compression_encode(const huffman_compression_t* self, char letter) {
    return huffman_tree_get_code(self->tree, letter);
}

void huffman_compression_destroy(huffman_compression_t *huffman) {
    huffman_tree_destroy(huffman->tree);
    free(huffman);
}

void huffman_compression_print_tree(huffman_compression_t *huffman) {
    huffman_tree_print(huffman->tree);
}