#ifndef C_HUFFMAN_TREE_H
#define C_HUFFMAN_TREE_H

struct huffman_tree;

typedef struct huffman_tree huffman_tree_t;

huffman_tree_t *huffman_tree_create();

void huffman_tree_destroy(huffman_tree_t *tree);

void huffman_tree_print(huffman_tree_t *tree);

#endif //C_HUFFMAN_TREE_H
