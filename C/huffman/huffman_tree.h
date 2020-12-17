#ifndef C_HUFFMAN_TREE_H
#define C_HUFFMAN_TREE_H

#include "huffman_node.h"

typedef struct huffman_tree {
    huffman_node_t *root;
}huffman_tree_t;

//Returns 0 if successful, otherwise returns -1
int huffman_tree_init(huffman_tree_t *tree);

void huffman_tree_release(huffman_tree_t *tree);

void huffman_tree_print(huffman_tree_t *tree);

#endif //C_HUFFMAN_TREE_H
