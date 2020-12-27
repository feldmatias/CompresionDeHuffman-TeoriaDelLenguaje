#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "decompression.h"
#include "../huffman/huffman_tree.h"
#include "bytes_vector/bytes_vector.h"
#include "../huffman/huffman.h"

#define COMPRESSED_EXTENSION ".huffman"
#define EXTENSION_CHAR "."
#define FILE_NAME_END "Decompressed.txt"
#define BITS_PER_BYTE 8

#define SUCCESS 0
#define BAD_FILE_NAME -1
#define MEMORY_ERROR -2
#define FILE_ERROR -3

/*
//Esta funcion tecnicamente no lo hace de la misma forma que en Rust, pero
//la puedo hacer de una forma mas conveniente, podriamos tomarlo como un ejemplo
//de que a veces el compilador se vuelve molesto, o que las reglas de rust pueden
//complicar las cosas
static long _extension_position(const char* file_name) {
    char* extension_ptr = strstr(file_name, COMPRESSED_EXTENSION);
    if (!extension_ptr) {
        return -1;
    }
    if ((extension_ptr - file_name) + strlen(extension_ptr) == strlen(file_name)) {
        return extension_ptr - file_name;
    }
    return -1;
}
*/

//Implemento la version que hace lo mismo que la de rust por las dudas
static bool _is_file_name_valid(const char* file_name) {
    char* extension_ptr = strstr(file_name, COMPRESSED_EXTENSION);
    if (!extension_ptr) {
        return false;
    }
    return (extension_ptr - file_name) + strlen(extension_ptr) == strlen(file_name);
}

static int _load_file(const char* file_name, char** compressed_file_string, long* file_len) {
    FILE* compressed_file = fopen(file_name, "r");
    if (!compressed_file) {
        return FILE_ERROR;
    }
    fseek(compressed_file, 0, SEEK_END);
    long compressed_file_size = ftell(compressed_file);
    *file_len = compressed_file_size;
    *compressed_file_string = malloc(sizeof(char) * (compressed_file_size + 1));
    if (!*compressed_file_string) {
        fclose(compressed_file);
        return MEMORY_ERROR;
    }

    //Cargo el archivo en memoria
    (*compressed_file_string)[compressed_file_size] = 0;
    fseek(compressed_file, 0, SEEK_SET);
    fread(*compressed_file_string, sizeof(char), compressed_file_size, compressed_file);

    //Podriamos "olvidarnos" de cerrar el archivo para leakear y dejar un file
    //descriptor suelto, y mostrar que en Rust esto no puede pasar
    fclose(compressed_file);
    return SUCCESS;
}

static int _get_char(const char* compressed_file_string, const huffman_compression_t* huff_tree,
              int* byte_to_read, char* read_bits, bytes_vector_t* decompressed_file) {
    char aux_byte, decoded_char;
    bool was_letter_decoded = false;
    bytes_vector_t tree_code;
    if (bytes_vector_init(&tree_code) != 0) {
        return MEMORY_ERROR;
    }

    while (!was_letter_decoded) {
        aux_byte = (unsigned char)(compressed_file_string[*byte_to_read] << (*read_bits)) >>
                    (*read_bits + BITS_PER_BYTE - (*read_bits + 1));
        if (bytes_vector_add_byte(&tree_code, aux_byte + '0') != 0) {
            return MEMORY_ERROR;
        }
        *read_bits += 1;
        decoded_char = huffman_compression_decode(huff_tree, bytes_vector_get_ptr(&tree_code));
        if (decoded_char != -1) {
            if (bytes_vector_add_byte(decompressed_file, decoded_char) != 0) {
                return MEMORY_ERROR;
            }
            was_letter_decoded = true;
        }
        if (*read_bits == BITS_PER_BYTE) {
            *read_bits = 0;
            (*byte_to_read) += 1;
        }
    }
    bytes_vector_release(&tree_code);
    return SUCCESS;
}

//Adds "Decompressed.txt" at the end of the bytes_vector
static int _attach_string(bytes_vector_t* vec, const char* str, int len) {
    for (int i = 0; i < len; ++i) {
        if (bytes_vector_add_byte(vec, str[i]) != 0) {
            bytes_vector_release(vec);
            return MEMORY_ERROR;
        }
    }
    return SUCCESS;
}


static int _write_to_file(const char* compressed_file_name, const bytes_vector_t* decompressed_file_text) {
    //Builds the name of the decompressed file
    bytes_vector_t decompressed_name;
    if (bytes_vector_init(&decompressed_name) != 0) {
        return MEMORY_ERROR;
    }
    if (_attach_string(&decompressed_name, compressed_file_name,
                      (int)(strstr(compressed_file_name, EXTENSION_CHAR) - compressed_file_name)) != SUCCESS) {
        bytes_vector_release(&decompressed_name);
        return MEMORY_ERROR;
    }
    if (_attach_string(&decompressed_name, FILE_NAME_END, strlen(FILE_NAME_END)) != SUCCESS) {
        bytes_vector_release(&decompressed_name);
        return MEMORY_ERROR;
    }

    //Creates the decompressed file and writes the data in it
    FILE* decompressed_file = fopen(bytes_vector_get_ptr(&decompressed_name), "w");
    if (!decompressed_file) {
        bytes_vector_release(&decompressed_name);
        return FILE_ERROR;
    }
    fwrite(bytes_vector_get_ptr(decompressed_file_text), sizeof(char),
           bytes_vector_len(decompressed_file_text), decompressed_file);
    fclose(decompressed_file);
    bytes_vector_release(&decompressed_name);
    return SUCCESS;
}

static int _build_decompressed_string(const char* compressed_file_string,
                                      int compressed_file_len, bytes_vector_t* decompressed_file_text,
                                      const huffman_compression_t* huffman) {
    char padding_bits = compressed_file_string[0];
    int byte_to_read = 1;
    char read_bits = 0;
    while (byte_to_read < compressed_file_len) {
        if (_get_char(compressed_file_string, huffman, &byte_to_read, &read_bits,
                     decompressed_file_text) != SUCCESS) {
            return MEMORY_ERROR;
        }
        if (byte_to_read >= compressed_file_len) {
            byte_to_read = compressed_file_len - 1;
        }
        if (byte_to_read == (compressed_file_len - 1)) {
            if ((BITS_PER_BYTE - read_bits) == padding_bits) {
                byte_to_read += 1;
            }
        }
    }
    return SUCCESS;
}

static int _execute_decompression(const char* file_name, const char* compressed_file_string, int compressed_file_len) {
    int program_status;
    huffman_compression_t huffman;
    if (huffman_compression_init(&huffman) != 0) {
        return MEMORY_ERROR;
    }
    bytes_vector_t decompressed_file_text;
    if (bytes_vector_init(&decompressed_file_text) != SUCCESS) {
        huffman_compression_release(&huffman);
        return MEMORY_ERROR;
    }
    program_status = _build_decompressed_string(compressed_file_string, compressed_file_len, &decompressed_file_text,
                                               &huffman);
    if (program_status != SUCCESS) {
        bytes_vector_release(&decompressed_file_text);
        huffman_compression_release(&huffman);
        return program_status;
    }
    program_status = _write_to_file(file_name, &decompressed_file_text);
    bytes_vector_release(&decompressed_file_text);
    huffman_compression_release(&huffman);
    return program_status;
}

int decompress_file(const char* file_name) {
    if (!_is_file_name_valid(file_name)) {
        return BAD_FILE_NAME;
    }
    char* compressed_file_string;
    long file_len = 0;
    int program_status = _load_file(file_name, &compressed_file_string, &file_len);

    //Esto falla si no se pudo llegar a alocar la memoria para el string (ej: no se puede
    //abrir el archivo)
    if (program_status != SUCCESS) {
        free(compressed_file_string);
        return program_status;
    }
    program_status = _execute_decompression(file_name, compressed_file_string, file_len);
    if (program_status != SUCCESS) {
        free(compressed_file_string);
        return program_status;
    }
    free(compressed_file_string);
    return SUCCESS;
}