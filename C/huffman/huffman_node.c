#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "huffman_node.h"
#include "../decompression/bytes_vector/bytes_vector.h"

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

bool huffman_node_is_leaf(const huffman_node_t *node) {
    return node->left == NULL && node->right == NULL;
}

/*
 * Given a code, it returns it's associated letter. If the code corresponds to a non-leaf node
 * (therefore it does not contain a letter) then it returns -1.
 */
char huffman_node_get_letter(const huffman_node_t* self, const char* tree_code_it) {
    if (huffman_node_is_leaf(self)) {
        return self->letter;
    } else {
        if (*tree_code_it != '\0') {
            if (*tree_code_it == '0') {
                return huffman_node_get_letter(self->left, ++tree_code_it);
            } else { //'1'
                return huffman_node_get_letter(self->right, ++tree_code_it);
            }
        } else {
            return -1;
        }
    }
}

/*
 * Recursively travels the tree in a CLR (center-left-right) manner until it finds (if it exists)
 * the node with the given letter, returning the code associated with the tree path. If the letter
 * is not present in any node, then it returns NULL. If the letter was found it will return an empty string,
 * subsequently adding the corresponding path to it for each recursive call.
 */
bytes_vector_t* huffman_node_get_code(huffman_node_t* self, char letter) {
    if (self->letter == letter) {
        bytes_vector_t* str = (bytes_vector_t*)calloc(1, sizeof(bytes_vector_t));
        if (!str || bytes_vector_init(str)) {
            fprintf(stderr, "Could not allocate memory for the code path!");
            exit(-1);
        }
        return str;
    } else if (huffman_node_is_leaf(self)) {
        return NULL;
    } else {
        bytes_vector_t* left_code = huffman_node_get_code(self->left, letter);
        if (!left_code) {
            bytes_vector_t* right_code = huffman_node_get_code(self->right, letter);
            if (!right_code) return NULL;
            bytes_vector_add_byte(right_code, '1');
            return right_code;
        } else {
            bytes_vector_add_byte(left_code, '0');
            return left_code;
        }
    }
}

void huffman_node_print(huffman_node_t *node) {
    if (huffman_node_is_leaf(node)) {
        printf("leaf: '%c' -> %d\n", node->letter, node->weight);
    } else {
        printf("not leaf: %d\n", node->weight);
    }

    if (node->left != NULL) {
        huffman_node_print(node->left);
    }
    if (node->right != NULL) {
        huffman_node_print(node->right);
    }
}

int huffman_node_weight(const huffman_node_t *node) {
    return node->weight;
}
