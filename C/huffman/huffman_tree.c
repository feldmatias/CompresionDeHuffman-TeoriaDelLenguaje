#include <stdlib.h>
#include <stdio.h>
#include "huffman_tree.h"
#include "huffman_node.h"
#include "nodes_list.h"

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

struct huffman_tree {
    huffman_node_t *root;
};

huffman_nodes_list_t* create_leave_nodes() {
    huffman_nodes_list_t* list = huffman_nodes_list_create();
    if (list == NULL) {
        return NULL;
    }

    for (int i = 0; i < 100; i++) {
        huffman_node_t *leave_node = huffman_node_create(CHARS[i], FREQUENCIES[i]);
        if (leave_node == NULL) {
            huffman_nodes_list_destroy(list);
            return NULL;
        }

        huffman_nodes_list_add_node(list, leave_node);
    }
    return list;
}

huffman_tree_t *huffman_tree_create() {
    huffman_nodes_list_t* list = create_leave_nodes();
    huffman_tree_t* tree = malloc(sizeof(huffman_tree_t));
    if (tree == NULL) {
        huffman_nodes_list_destroy(list);
        return NULL;
    }

    while (huffman_nodes_list_length(list) > 1) {
        huffman_node_t* node1 = huffman_nodes_list_get_min_node(list);
        huffman_node_t* node2 = huffman_nodes_list_get_min_node(list);
        huffman_node_t* merged_node = huffman_node_merge_nodes(node1, node2);

        if (merged_node == NULL) {
            huffman_nodes_list_destroy(list);
            free(tree);
            return NULL;
        }

        huffman_nodes_list_add_node(list, merged_node);
    }

    huffman_node_t* root = huffman_nodes_list_get_min_node(list); // last node
    huffman_nodes_list_destroy(list);

    tree->root = root;
    return tree;
}

void huffman_tree_destroy(huffman_tree_t *tree) {
    huffman_node_destroy(tree->root);
    free(tree);
}

void print(huffman_tree_t *tree) {
    printf("_____________");
    printf("Huffman Tree");
    huffman_node_print(tree->root);
    printf("_____________");
}
