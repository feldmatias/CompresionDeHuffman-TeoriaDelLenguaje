#include "compression.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "stdlib.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/huffman.h"
#include "../decompression/bytes_vector/bytes_vector.h"

bool valid_file_extension(const char* file_name) {
    char* extension = strstr(file_name, ".txt");
    if (!extension) {
        return false;
    }
    return true;
}

char string_to_char(const bytes_vector_t* byte_buffer) {
    char c = 0;
    const char* buf_ptr = bytes_vector_get_ptr(byte_buffer);
    for (int i = 0; i < 8; ++i) {
        c += (char)pow(2, i) * (*(buf_ptr + 7 - i) & 0x00000001);
    }
    return c;
}

void add_bytes(bytes_vector_t* bytes_vector_to, bytes_vector_t* bytes_vector_from, int len) {
    const char* code_ptr = bytes_vector_get_ptr(bytes_vector_from);
    for (int i = 0; i < len; ++i) {
        bytes_vector_add_byte(bytes_vector_to, *(code_ptr + i));
    }
}

void add_compression_byte(bytes_vector_t * byte_buffer, bytes_vector_t * compression_vec) {
    char curr_char = string_to_char(byte_buffer);
    bytes_vector_add_byte(compression_vec, curr_char);
    bytes_vector_release(byte_buffer);
    bytes_vector_init(byte_buffer);
}

//Hacer q devuelva int como codigo de error
void process_code(char* bits_left, bytes_vector_t* byte_buffer, bytes_vector_t* code, bytes_vector_t* compression_vec) {
    bool finished_code = false;
    char code_len = (char)bytes_vector_len(code) - 1;//Uso char porque no va a haber codigos de mas de 256 caracteres
    int index = 0;
    const char* code_ptr = bytes_vector_get_ptr(code);

    while (!finished_code) {
        if (*bits_left - code_len >= 0) {
            *bits_left -= code_len;
            add_bytes(byte_buffer, code, code_len);
            finished_code = true;
        } else if (*bits_left - code_len < 0) {
            add_bytes(byte_buffer, code, *bits_left);
            index += *bits_left;

            for (int i = 0; i < code_len - *bits_left; ++i) {
                bytes_vector_replace_byte(code, i, *(code_ptr + index + i));
            }
            code_len -= *bits_left;
            code_ptr = bytes_vector_get_ptr(code);
            index = 0;
            *bits_left = 0;
        }
        if (*bits_left == 0) {
            add_compression_byte(byte_buffer, compression_vec);
            *bits_left = 8;
        }
    }
}

void add_padding(const char* bits_left, bytes_vector_t* byte_buffer, bytes_vector_t* compression_vec) {
    for (int i = 0; i < *bits_left; ++i) {
        bytes_vector_add_byte(byte_buffer, '0');
    }
    char curr_char = string_to_char(byte_buffer);
    bytes_vector_add_byte(compression_vec, curr_char);

    bytes_vector_replace_byte(compression_vec,0, *bits_left);//Ver si chequeo error
}

int process_file(const char* file_name, bytes_vector_t* compression_vec, char* bits_left) {
    FILE* file_to_compress = fopen(file_name, "r");
    if (!file_to_compress) {
        fprintf(stderr, "Could not open file, check if it exists in the directory of the binary!\n");
        return -2;//Could not open file
    }

    huffman_compression_t huffman;
    if (huffman_compression_init(&huffman) != 0) return -3;//Memory Error

    bytes_vector_t byte_buffer;
    if (bytes_vector_init(&byte_buffer) != 0) return -3;

    char c = (char)getc(file_to_compress);
    bytes_vector_t* code;

    while (!feof(file_to_compress)) {
        code = huffman_compression_encode(&huffman, c);//Esto me esta devolviendo un bytes_vector de tamaño 320
        if (!code) {
            fprintf(stderr, "Inexistent letter! Check file's letter frequencies\n");
            return -4;//Letra inexistente
        }
        process_code(bits_left, &byte_buffer, code, compression_vec);
        c = (char)getc(file_to_compress);
        bytes_vector_release(code);
        free(code);
    }

    add_padding(bits_left, &byte_buffer, compression_vec);

    bytes_vector_release(&byte_buffer);
    huffman_compression_release(&huffman);
    fclose(file_to_compress);
}

int write_to_file(const char* file_name, bytes_vector_t* compression_vec) {
    bytes_vector_t compressed_file_name;
    if (bytes_vector_init(&compressed_file_name) != 0) return -1;

    for (int i = 0; i < strlen(file_name) - 4; ++i) {
        bytes_vector_add_byte(&compressed_file_name, *(file_name + i));
    }

    const char* compressed_extension = ".huffman";
    for (int i = 0; i < strlen(compressed_extension); ++i) {
        bytes_vector_add_byte(&compressed_file_name, *(compressed_extension + i));
    }

    FILE* compressed = fopen(bytes_vector_get_ptr(&compressed_file_name), "w");
    const char* vec_ptr = bytes_vector_get_ptr(compression_vec);
    fwrite(vec_ptr, sizeof(char), bytes_vector_len(compression_vec), compressed);
    bytes_vector_release(&compressed_file_name);
    fclose(compressed);
}

int compress_file(const char* file_name) {
    if(!valid_file_extension(file_name)) {
        fprintf(stderr, "Invalid file extension! It must be .txt");
        return -1;//Invalid Extension
    }

    bytes_vector_t compression_vec;
    if (bytes_vector_init(&compression_vec) != 0) {
        fprintf(stderr, "Memory error!");
        return -3;
    }
    bytes_vector_add_byte(&compression_vec, '0');//Aca despues meto la cantidad de bits de padding

    char bits_left = 8;//Uso char porque int es demasiado grande

    int status = process_file(file_name, &compression_vec, &bits_left);
    if (status != 0){
        bytes_vector_release(&compression_vec);
        fprintf(stderr, "Unknown error when processing file!");
        return -4;//algun error
    }

    write_to_file(file_name, &compression_vec);

    bytes_vector_release(&compression_vec);
    return 0;
}