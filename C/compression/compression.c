#include "compression.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "stdlib.h"
#include "../huffman/huffman_tree.h"
#include "../huffman/huffman.h"
#include "../decompression/bytes_vector/bytes_vector.h"

#define SUCCESS 0
#define INVALID_EXTENSION -1
#define FILE_ERROR -2
#define MEMORY_ALLOCATION_ERROR -3
#define INEXISTENT_LETTER -4

#define ORIGINAL_EXT  ".txt"
#define ORIGINAL_EXT_LEN 4
#define COMPRESSED_EXT ".huffman"

#define PADDING '0'

#define READ "r"
#define WRITE "w"


bool valid_file_extension(const char* file_name) {
    char* extension = strstr(file_name, ORIGINAL_EXT);
    if (!extension) {
        return false;
    }
    return true;
}

char string_to_char(const bytes_vector_t* byte_buffer) {
    char c = 0;
    const char* buf_ptr = bytes_vector_get_ptr(byte_buffer) + 7;// el + 7 es para apuntar al bit menos significativo
    for (int i = 0; i < 8; ++i) {
        c += (char)pow(2, i) * (*(buf_ptr - i) & 1);//El AND con 1 es para tomar solo el bit menos significativo,
                                                        // que me dice si el byte actual es 0 o 1 ya que estoy trabajando
                                                        // con un vector de chars
    }
    return c;
}

void add_bytes(bytes_vector_t* to, bytes_vector_t* from, int len) {
    const char* code_ptr = bytes_vector_get_ptr(from);
    for (int i = 0; i < len; ++i) {
        bytes_vector_add_byte(to, *(code_ptr + i));
    }
}

int add_compressed_byte(bytes_vector_t * byte_buffer, bytes_vector_t * compression_vec) {
    char curr_char = string_to_char(byte_buffer);
    if (bytes_vector_add_byte(compression_vec, curr_char) != SUCCESS) return MEMORY_ALLOCATION_ERROR;
    bytes_vector_release(byte_buffer);
    if (bytes_vector_init(byte_buffer) != SUCCESS) return MEMORY_ALLOCATION_ERROR;
    //Hago release y despues init para limpiar el byte_buffer

    return SUCCESS;
}

int process_code(int* bits_left, bytes_vector_t* byte_buffer, bytes_vector_t* code, bytes_vector_t* compression_vec) {
    bool finished_code = false;
    int code_len = bytes_vector_len(code) - 1;
    int index = 0;
    const char* code_ptr = bytes_vector_get_ptr(code);

    while (!finished_code) {
        if (*bits_left - code_len >= 0) {
            *bits_left -= code_len;
            add_bytes(byte_buffer, code, code_len);//Aca no chequeo si falla el llamado a add_byte ya que nunca va a tener
                                                   //que hacer un realloc porque a lo sumo agrego 8 bytes, y el tamaño inicial es 10
            finished_code = true;
        } else if (*bits_left - code_len < 0) {
            add_bytes(byte_buffer, code, *bits_left);//Igual que en la linea 53
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
            if (add_compressed_byte(byte_buffer, compression_vec) != SUCCESS) return MEMORY_ALLOCATION_ERROR;
            *bits_left = 8;
        }
    }
    return SUCCESS;
}

int add_padding(const int* bits_left, bytes_vector_t* byte_buffer, bytes_vector_t* compression_vec) {
    for (int i = 0; i < *bits_left; ++i) {
        bytes_vector_add_byte(byte_buffer, PADDING);//No chequeo error aca porque nunca voy a hacer realloc
    }
    char curr_char = string_to_char(byte_buffer);
    if (bytes_vector_add_byte(compression_vec, curr_char) != SUCCESS) return MEMORY_ALLOCATION_ERROR;

    bytes_vector_replace_byte(compression_vec,0, (char)*bits_left);

    return SUCCESS;
}

int process_file(const char* file_name, bytes_vector_t* compression_vec, int* bits_left) {
    FILE* file_to_compress = fopen(file_name, READ);
    if (!file_to_compress) {
        fprintf(stderr, "Could not open file\n");
        return FILE_ERROR;
    }

    huffman_compression_t huffman;
    if (huffman_compression_init(&huffman) != SUCCESS) {
        fprintf(stderr, "Could not create Huffman tree\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    bytes_vector_t byte_buffer;
    if (bytes_vector_init(&byte_buffer) != SUCCESS) {
        fprintf(stderr, "Could not create bytes vector\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    char c = (char)getc(file_to_compress);
    bytes_vector_t* code;

    while (!feof(file_to_compress)) {
        code = huffman_compression_encode(&huffman, c);//Esto me esta devolviendo un bytes_vector de tamaño 320
        if (!code) {
            fprintf(stderr, "Inexistent letter!\n");
            return INEXISTENT_LETTER;
        }
        int status = process_code(bits_left, &byte_buffer, code, compression_vec);
        if(status != SUCCESS){
            fprintf(stderr, "Error processing code\n");
            return status;
        }
        c = (char)getc(file_to_compress);
        bytes_vector_release(code);
        free(code);
    }

    if (add_padding(bits_left, &byte_buffer, compression_vec) != SUCCESS) return -3;

    bytes_vector_release(&byte_buffer);
    huffman_compression_release(&huffman);
    fclose(file_to_compress);

    return SUCCESS;
}

int write_to_file(const char* file_name, bytes_vector_t* compression_vec) {
    bytes_vector_t compressed_file_name;
    if (bytes_vector_init(&compressed_file_name) != SUCCESS) return MEMORY_ALLOCATION_ERROR;

    for (int i = 0; i < strlen(file_name) - ORIGINAL_EXT_LEN; ++i) {
        bytes_vector_add_byte(&compressed_file_name, *(file_name + i));
    }

    const char* compressed_extension = COMPRESSED_EXT;
    for (int i = 0; i < strlen(compressed_extension); ++i) {
        bytes_vector_add_byte(&compressed_file_name, *(compressed_extension + i));
    }

    FILE* compressed = fopen(bytes_vector_get_ptr(&compressed_file_name), WRITE);
    const char* vec_ptr = bytes_vector_get_ptr(compression_vec);
    fwrite(vec_ptr, sizeof(char), bytes_vector_len(compression_vec), compressed);
    bytes_vector_release(&compressed_file_name);
    fclose(compressed);

    return SUCCESS;
}

int compress_file(const char* file_name) {
    if(!valid_file_extension(file_name)) {
        fprintf(stderr, "Invalid file extension! It must be .txt\n");
        return INVALID_EXTENSION;
    }

    bytes_vector_t compression_vec;
    if (bytes_vector_init(&compression_vec) != SUCCESS) {
        fprintf(stderr, "Memory error!\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    bytes_vector_add_byte(&compression_vec, '0');//Agrego este byte para despues reemplazarlo con la cantidad de bits de padding

    int bits_left = 8;

    int status = process_file(file_name, &compression_vec, &bits_left);
    if (status != SUCCESS){
        bytes_vector_release(&compression_vec);
        fprintf(stderr, "Error processing file!\n");
        return status;
    }

    status = write_to_file(file_name, &compression_vec);
    if (status != SUCCESS){
        bytes_vector_release(&compression_vec);
        fprintf(stderr, "Error writing compressed file!\n");
        return status;
    }

    bytes_vector_release(&compression_vec);

    return SUCCESS;
}