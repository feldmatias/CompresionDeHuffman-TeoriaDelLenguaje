#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "decompression.h"
#include "../huffman/huffman_tree.h"
#include "bytes_vector/bytes_vector.h"
#include "../huffman/huffman.h"

#define COMPRESSED_EXTENSION ".huffman"
#define DECOMPRESSED_EXTENSION ".txt"
#define EXTENSION_CHAR "."
#define BITS_PER_BYTE 8

#define SUCCESS 0
#define BAD_FILE_NAME -1
#define MEMORY_ERROR -2
#define FILE_ERROR -3

//Esta funcion tecnicamente no lo hace de la misma forma que en Rust, pero
//la puedo hacer de una forma mas conveniente, podriamos tomarlo como un ejemplo
//de que a veces el compilador se vuelve molesto, o que las reglas de rust pueden
//complicar las cosas
long extension_position(const char* file_name) {
    char* extension_ptr = strstr(file_name, COMPRESSED_EXTENSION);
    if (!extension_ptr) {
        return -1;
    }
    if ((extension_ptr - file_name) + strlen(extension_ptr) == strlen(file_name)) {
        return extension_ptr - file_name;
    }
    return -1;
}

//Implemento la version que hace lo mismo que la de rust por las dudas
bool is_file_name_valid(const char* file_name) {
    char* extension_ptr = strstr(file_name, COMPRESSED_EXTENSION);
    if (!extension_ptr) {
        return false;
    }
    return (extension_ptr - file_name) + strlen(extension_ptr) == strlen(file_name);
}

int load_file(const char* file_name, char** compressed_file_string, int* file_len) {
    FILE* compressed_file = fopen(file_name, "r");
    if (!compressed_file) {
        return FILE_ERROR;
    }
    fseek(compressed_file, 0, SEEK_END);
    long compressed_file_size = ftell(compressed_file);
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

//Returns the number of read bits
int get_char(const char* compressed_file_string, const huffman_compression_t* huff_tree,
              int* byte_to_read, char* read_bits, bytes_vector_t* decompressed_file) {
    char aux_byte, decoded_char;
    bool was_letter_decoded = false;
    bytes_vector_t tree_code;
    if (bytes_vector_init(&tree_code) != 0) {
        return MEMORY_ERROR;
    }

    while (!was_letter_decoded) {
        aux_byte = (compressed_file_string[*byte_to_read] << (*read_bits)) >>
                    (*read_bits + BITS_PER_BYTE - (*read_bits + 1));
        if (bytes_vector_add_byte(&tree_code, aux_byte + '0') != 0) {
            return MEMORY_ERROR;
        }

        *read_bits += 1;
        decoded_char = huffman_compression_decode(huff_tree, bytes_vector_get_ptr(&tree_code));
        if (decoded_char != -1) {
            if (bytes_vector_add_byte(&tree_code, decoded_char) != 0) {
                return MEMORY_ERROR;
            }
            was_letter_decoded = true;
        }
        if (*read_bits == BITS_PER_BYTE) {
            *read_bits = 0;
            (*byte_to_read) += 1;
        }
    }

    return SUCCESS;
}

int execute_decompression(const char* file_name, const char* compressed_file_string, int compressed_file_len) {
    char padding_bits = compressed_file_string[0];
    int byte_to_read = 1;
    int last_byte_to_read = 1;
    char read_bits = 0;
    char last_byte_read_bits = 0;
    huffman_compression_t huffman;
    if (huffman_compression_init(&huffman) != 0) {
        return MEMORY_ERROR;
    }
    bytes_vector_t decompressed_file_text;
    if (bytes_vector_init(&decompressed_file_text) != SUCCESS) {
        huffman_compression_release(&huffman);
        return MEMORY_ERROR;
    }
    while (byte_to_read < compressed_file_len) {
        if (get_char(compressed_file_string, &huffman, &byte_to_read, &read_bits,
                     &decompressed_file_text) != SUCCESS) {
            bytes_vector_release(&decompressed_file_text);
            huffman_compression_release(&huffman);
            return MEMORY_ERROR;
        }
        if (byte_to_read == last_byte_to_read) {
            last_byte_read_bits += read_bits;
        } else {
            last_byte_read_bits = 0;
        }
        last_byte_to_read = byte_to_read;
        if (byte_to_read == (compressed_file_len - 1)) {
            if ((BITS_PER_BYTE - last_byte_read_bits) == padding_bits) {
                byte_to_read += 1;
            }
        }
    }

    //crear archivo y escribirlo

    bytes_vector_release(&decompressed_file_text);
    huffman_compression_release(&huffman);
    return SUCCESS;
}

int write_to_file(const char* file_name, const bytes_vector_t* decompressed_file_text) {
    bytes_vector_t decompressed_name;
    if (bytes_vector_init(&decompressed_name) != 0) {
        return MEMORY_ERROR;
    }
    long file_name_end = strstr(file_name, EXTENSION_CHAR) - file_name;
    for (int i = 0; i < file_name_end; ++i) {
        if (bytes_vector_add_byte(&decompressed_name, file_name[file_name_end]) != 0) {
            bytes_vector_release(&decompressed_name);
            return MEMORY_ERROR;
        }
    }
    FILE* decompressed_file = fopen(bytes_vector_get_ptr(&decompressed_name), "w");

    //SEGUIR IMPLEMENTANDO FUNCION

    return SUCCESS;
}

int decompress_file(const char* file_name) {
    if (!is_file_name_valid(file_name)) {
        return BAD_FILE_NAME;
    }
    char* compressed_file_string;
    int file_len = 0;
    int program_status = load_file(file_name, &compressed_file_string, &file_len);
    if (program_status != SUCCESS) {
        free(compressed_file_string);
        return program_status;
    }
    program_status = execute_decompression(file_name, compressed_file_string, file_len);
    if (program_status != SUCCESS) {
        free(compressed_file_string);
        return program_status;
    }
    //Pasar a una funcion que lo libera para que el programa quede simetrico
    free(compressed_file_string);




    //CAMBIAR ESTO PORQUE PUEDO HACER QUE LA FUNCION QUE CHEQUEA EL NOMBRE DEL ARCHIVO RETORNE LA POSICION
    //DE LA EXTENSION, TENDRIA QUE SACAR EL EXPECT DEL find
    let mut decompressed_file = fs::File::create(file_name[..file_name.find(COMPRESSED_EXTENSION).
            expect("Wrong extension")].to_string() + DECOMPRESSED_EXTENSION).
    expect("Problem creating the file");
    decompressed_file.write_all( decompressed_file_text.as_bytes()).expect("Problem writing to file");

    return SUCCESS;
}