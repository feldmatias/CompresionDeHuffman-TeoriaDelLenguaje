#ifndef C_HUFFMAN_TREE_H
#define C_HUFFMAN_TREE_H

#include "huffman_node.h"

typedef struct huffman_tree {
    huffman_node_t* root;
} huffman_tree_t;

//Returns 0 if successful, otherwise returns -1
int huffman_tree_init(huffman_tree_t *tree);

/*
 * Given a code, it returns it's associated letter. If the code corresponds to a non-leaf node
 * (therefore it does not contain a letter) then it returns -1.
 */
char huffman_tree_get_letter(const huffman_tree_t* self, const char* tree_code);

/*
 * Given a letter, returns the corresponded code. If the letter does not exist it returns
 * an empty string.
 */
bytes_vector_t* huffman_tree_get_code(const struct huffman_tree* self, char letter);

void huffman_tree_release(huffman_tree_t *tree);

void huffman_tree_print(huffman_tree_t *tree);

#endif //C_HUFFMAN_TREE_H
