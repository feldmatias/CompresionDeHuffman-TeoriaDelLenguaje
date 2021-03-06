#include <stdio.h>
#include "huffman_tree.h"
#include "huffman_node.h"
#include "nodes_list.h"
#include <string.h>

#define SUCCESS 0
#define MEMORY_ERROR -1;

const char CHARS[100] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*',
        '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`',
        '{', '|', '}', '~', ' ', '\t', '\n', '\r', '\x0b', '\x0c',
};

const int FREQUENCIES[100] = {
        325, 982, 373, 343, 107, 94, 73, 45, 47, 966, 245509, 46768, 67194, 134216, 406157, 69103,
        57328, 218875, 199130, 2788, 29345, 146532, 95890, 216805, 282560, 46849, 2414, 209907,
        215605, 291243, 115225, 34077, 73155, 4681, 85549, 1098, 44560, 15450, 21526, 15726, 42703,
        11770, 11261, 18486, 55893, 2071, 6206, 23919, 15887, 27393, 33268, 12038, 1179, 29040, 34082,
        39878, 14169, 3587, 16508, 608, 9128, 532, 8840, 488, 3, 2, 1, 21, 31077, 643, 644, 73, 0,
        83315, 8150, 78216, 48, 1843, 17199, 468, 0, 440, 10476, 3, 2076, 0, 2068, 0, 71, 1, 0, 33,
        2, 1, 1285884, 0, 124787, 0, 0, 0,
};


huffman_nodes_list_t *create_leaf_nodes() {
    huffman_nodes_list_t *list = huffman_nodes_list_create();
    if (list == NULL) {
        return NULL;
    }

    for (int i = 0; i < 100; i++) {
        huffman_node_t *leaf_node = huffman_node_create(CHARS[i], FREQUENCIES[i]);
        if (leaf_node == NULL) {
            huffman_nodes_list_destroy(list);
            return NULL;
        }

        huffman_nodes_list_add_node(list, leaf_node);
    }
    return list;
}

int huffman_tree_init(huffman_tree_t *tree) {
    huffman_nodes_list_t *list = create_leaf_nodes();
    if (list == NULL) {
        return MEMORY_ERROR;
    }
    while (huffman_nodes_list_length(list) > 1) {
        huffman_node_t *node1 = huffman_nodes_list_get_min_node(list);
        huffman_node_t *node2 = huffman_nodes_list_get_min_node(list);
        huffman_node_t *merged_node = huffman_node_merge_nodes(node1, node2);

        if (merged_node == NULL) {
            huffman_nodes_list_destroy(list);
            return MEMORY_ERROR;
        }

        huffman_nodes_list_add_node(list, merged_node);
    }

    huffman_node_t *root = huffman_nodes_list_get_min_node(list); // last node
    huffman_nodes_list_destroy(list);

    tree->root = root;
    return SUCCESS;
}

char huffman_tree_get_letter(const huffman_tree_t* self, const char* tree_code) {
    return huffman_node_get_letter(self->root, tree_code);
}

bytes_vector_t* huffman_tree_get_code(const struct huffman_tree* self, char letter) {
    return huffman_node_get_code(self->root, letter);
}

void huffman_tree_release(huffman_tree_t *tree) {
    huffman_node_destroy(tree->root);
}

void huffman_tree_print(huffman_tree_t *tree) {
    printf("_____________\n");
    printf("Huffman Tree\n");
    printf("_____________\n");
    huffman_node_print(tree->root);
    printf("_____________\n");
}
