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

void huffman_compression_destroy(huffman_compression_t *huffman) {
    huffman_tree_destroy(huffman->tree);
    free(huffman);
}

void huffman_compression_print_tree(huffman_compression_t *huffman) {
    huffman_tree_print(huffman->tree);
}