#ifndef C_HUFFMAN_NODE_H
#define C_HUFFMAN_NODE_H

#include <stdbool.h>

struct huffman_node;

typedef struct huffman_node huffman_node_t;


huffman_node_t *huffman_node_create(char letter, int frequency);

void huffman_node_destroy(huffman_node_t *node);

huffman_node_t *huffman_node_merge_nodes(huffman_node_t *node1, huffman_node_t *node2);

bool huffman_node_is_leave(huffman_node_t *node);

void huffman_node_print(huffman_node_t *node);

int huffman_node_weight(huffman_node_t *node);

#endif //C_HUFFMAN_NODE_H
