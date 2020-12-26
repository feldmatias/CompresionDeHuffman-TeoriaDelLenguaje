#include <stdio.h>
#include <string.h>
#include "huffman/huffman.h"
#include "compression/compression.h"
#include "decompression/decompression.h"

#define ARGS_AMOUNT 3
#define PROGRAM_MODE_INDEX 1
#define FILE_INDEX 2
#define PRINT_TREE_ARGS_AMOUNT 2
#define DECOMPRESS_ARG "decompress"
#define COMPRESS_ARG "compress"
#define PRINT_TREE "print"

int main(int argc, const char* argv[]) {
    if (argc == ARGS_AMOUNT) {
        if (!strcmp(argv[PROGRAM_MODE_INDEX], COMPRESS_ARG)) {
            compress_file(argv[FILE_INDEX]);
        } else if (!strcmp(argv[PROGRAM_MODE_INDEX], DECOMPRESS_ARG)) {
            decompress_file(argv[FILE_INDEX]);
            return 0;
        } else {
            printf("Incorrect argument\n");
            return -1;
        }
    } else if (argc == PRINT_TREE_ARGS_AMOUNT) {
        if (!strcmp(argv[PROGRAM_MODE_INDEX], PRINT_TREE)) {
            huffman_compression_t huffman;
            if (huffman_compression_init(&huffman) != 0) {
                return -1;
            }
            huffman_compression_print_tree(&huffman);
            huffman_compression_release(&huffman);
        } else {
            printf("Incorrect argument\n");
            return -1;
        }
    } else {
        printf("Incorrect amount of parameters\n");
        return -1;
    }
    /*
    huffman_compression_t *huffman = huffman_compression_create();
    if (huffman == NULL) {
        fprintf(stderr, "An error occurred creating Huffman Compression Algorithm\n");
        return 1;
    }

    huffman_compression_print_tree(huffman);

    huffman_compression_destroy(huffman);
    huffman_compression_t huffman;
    if (huffman_compression_init(&huffman) != 0) {
        fprintf(stderr, "An error occurred creating Huffman Compression Algorithm\n");
        return 1;
    }
    huffman_compression_print_tree(&huffman);
    huffman_compression_release(&huffman);
    */

    return 0;
}
