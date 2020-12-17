#ifndef C_HUFFMAN_TREE_H
#define C_HUFFMAN_TREE_H

#include "../decompression/bytes_vector/bytes_vector.h"

struct huffman_tree;

typedef struct huffman_tree huffman_tree_t;

huffman_tree_t *huffman_tree_create();

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

void huffman_tree_destroy(huffman_tree_t *tree);

void huffman_tree_print(huffman_tree_t *tree);

#endif //C_HUFFMAN_TREE_H
