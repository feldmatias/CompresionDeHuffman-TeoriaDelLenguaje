
#include "huffman_node.h"

#ifndef C_NODES_LIST_H
#define C_NODES_LIST_H

struct huffman_nodes_list;

typedef struct huffman_nodes_list huffman_nodes_list_t;


huffman_nodes_list_t *huffman_nodes_list_create();

void huffman_nodes_list_destroy(huffman_nodes_list_t *list);

void huffman_nodes_list_add_node(huffman_nodes_list_t *list, huffman_node_t *node);

huffman_node_t *huffman_nodes_list_get_min_node(huffman_nodes_list_t *list);

int huffman_nodes_list_length(huffman_nodes_list_t *list);

#endif //C_NODES_LIST_H
