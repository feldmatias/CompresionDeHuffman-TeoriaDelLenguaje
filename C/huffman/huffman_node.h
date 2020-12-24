#ifndef C_HUFFMAN_NODE_H
#define C_HUFFMAN_NODE_H

#include <stdbool.h>

struct huffman_node;

typedef struct huffman_node huffman_node_t;
typedef struct bytes_vector bytes_vector_t;

huffman_node_t *huffman_node_create(char letter, int frequency);

void huffman_node_destroy(huffman_node_t *node);

huffman_node_t *huffman_node_merge_nodes(huffman_node_t *node1, huffman_node_t *node2);

bool huffman_node_is_leaf(const huffman_node_t *node);

/*
 * Given a code, it returns it's associated letter. If the code corresponds to a non-leaf node
 * (therefore it does not contain a letter) then it returns -1.
 */
char huffman_node_get_letter(const huffman_node_t* self, const char* tree_code_it);

/*
 * Recursively travels the tree in a CLR (center-left-right) manner until it finds (if it exists)
 * the node with the given letter, returning the code associated with the tree path. If the letter
 * is not present in any node, then it returns NULL. If the letter was found it will return an empty string,
 * subsequently adding the corresponding path to it for each recursive call.
 */
bytes_vector_t* huffman_node_get_code(huffman_node_t* self, char letter);

void huffman_node_print(huffman_node_t *node);

int huffman_node_weight(const huffman_node_t *node);

#endif //C_HUFFMAN_NODE_H
