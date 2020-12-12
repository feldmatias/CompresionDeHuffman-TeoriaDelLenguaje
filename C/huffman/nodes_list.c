#include <stdlib.h>
#include <limits.h>
#include "nodes_list.h"
#include "huffman_node.h"

const int SIZE = 100;

// Helper class to store temporary nodes and easily get the ones with minimum weight
struct huffman_nodes_list {
    huffman_node_t *nodes[100];
    int length;
};

huffman_nodes_list_t *huffman_nodes_list_create() {
    huffman_nodes_list_t *list = calloc(1, sizeof(huffman_nodes_list_t));
    return list;
}

void huffman_nodes_list_destroy(huffman_nodes_list_t *list) {
    for (int i = 0; i < SIZE; i++) {
        if (list->nodes[i] != NULL) {
            huffman_node_destroy(list->nodes[i]);
        }
    }
    free(list);
}


void huffman_nodes_list_add_node(huffman_nodes_list_t *list, huffman_node_t *node) {
    for (int i = 0; i < SIZE; i++) {
        if (list->nodes[i] == NULL) {
            list->nodes[i] = node;
            list->length++;
            return;
        }
    }
}

huffman_node_t *huffman_nodes_list_get_min_node(huffman_nodes_list_t *list) {
    int min_index = 0;
    int min_weight = INT_MAX;

    for (int i = 0; i < SIZE; i++) {
        if (list->nodes[i] == NULL) {
            continue;
        }

        int weight = huffman_node_weight(list->nodes[i]);
        if (weight < min_weight) {
            min_index = i;
            min_weight = weight;
        }
    }

    huffman_node_t *node = list->nodes[min_index];
    list->nodes[min_index] = NULL;
    list->length--;
    return node;
}

int huffman_nodes_list_length(huffman_nodes_list_t *list) {
    return list->length;
}

