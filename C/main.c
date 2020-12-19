#include <stdio.h>
#include "huffman/huffman.h"

int main() {
    /*
    huffman_compression_t *huffman = huffman_compression_create();
    if (huffman == NULL) {
        fprintf(stderr, "An error occurred creating Huffman Compression Algorithm\n");
        return 1;
    }

    huffman_compression_print_tree(huffman);

    huffman_compression_destroy(huffman);
    */
    huffman_compression_t huffman;
    if (huffman_compression_init(&huffman) != 0) {
        fprintf(stderr, "An error occurred creating Huffman Compression Algorithm\n");
        return 1;
    }
    huffman_compression_print_tree(&huffman);
    huffman_compression_release(&huffman);
    return 0;
}
