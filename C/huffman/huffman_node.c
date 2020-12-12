#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "huffman_node.h"

struct huffman_node {
    char letter;
    int weight;
    huffman_node_t *left;
    huffman_node_t *right;
};


huffman_node_t *huffman_node_create(char letter, int frequency) {
    huffman_node_t *node = malloc(sizeof(huffman_node_t));
    if (!node) {
        return NULL;
    }

    node->letter = letter;
    node->weight = frequency;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void huffman_node_destroy(huffman_node_t *node) {
    if (node->left != NULL) {
        huffman_node_destroy(node->left);
    }
    if (node->right != NULL) {
        huffman_node_destroy(node->right);
    }

    free(node);
}

huffman_node_t *huffman_node_merge_nodes(huffman_node_t *node1, huffman_node_t *node2) {
    int w1 = node1->weight;
    int w2 = node2->weight;
    huffman_node_t *left;
    huffman_node_t *right;

    // lower to left, higher to right
    if (w1 < w2) {
        left = node1;
        right = node2;
    } else {
        left = node2;
        right = node1;
    }

    huffman_node_t *new_node = huffman_node_create(0, w1 + w2);
    if (new_node == NULL) {
        return NULL;
    }

    new_node->left = left;
    new_node->right = right;
    return new_node;
}

bool huffman_node_is_leave(huffman_node_t *node) {
    return node->left == NULL && node->right == NULL;
}

void huffman_node_print(huffman_node_t *node) {
    if (huffman_node_is_leave(node)) {
        printf("leave: '%c' -> %d\n", node->letter, node->weight);
    } else {
        printf("not leave: %d\n", node->weight);
    }

    if (node->left != NULL) {
        huffman_node_print(node->left);
    }
    if (node->right != NULL) {
        huffman_node_print(node->right);
    }
}

int huffman_node_weight(huffman_node_t *node) {
    return node->weight;
}
