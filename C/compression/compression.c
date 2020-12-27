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

//Hacer q devuelva int como codigo de error
void process_code(char* bits_left, bytes_vector_t* byte_buffer, bytes_vector_t* code, bytes_vector_t* compression_vec) {
    bool finished_char = false;
    char code_len = (char)bytes_vector_len(code) - 1;//Uso char porque no va a haber codigos de mas de 256 caracteres
    int index = 0;
    const char* code_ptr = bytes_vector_get_ptr(code);

    while (!finished_char) {
        if (*bits_left - code_len >= 0) {
            *bits_left -= code_len;
            //byte_buffer.push_str(code);
            for (int i = 0; i < code_len; ++i) {
                bytes_vector_add_byte(byte_buffer, *(code_ptr + i));
            }
            finished_char = true;
        } else if (*bits_left - code_len < 0) {
            //aux = code.get(0..*bits_left as usize).unwrap();
            //byte_buffer.push_str(aux);

            for (int i = 0; i < *bits_left; ++i) {
                bytes_vector_add_byte(byte_buffer, *(code_ptr + i));
            }
            index += *bits_left;
            //*code = code.get(*bits_left as usize..code_len as usize).unwrap().to_string();
            bytes_vector_t aux;
            bytes_vector_init(&aux);//chequear error de memoria aca
            for (int i = 0; i < code_len - *bits_left; ++i) {
                bytes_vector_add_byte(&aux, *(code_ptr + index + i));
            }
            bytes_vector_release(code);
            bytes_vector_init(code);
            const char* aux_ptr = bytes_vector_get_ptr(&aux);
            for (int i = 0; i < code_len - *bits_left; ++i) {
                bytes_vector_add_byte(code, *(aux_ptr + i));
            }
            bytes_vector_release(&aux);
            code_len = (char)bytes_vector_len(code);
            code_ptr = bytes_vector_get_ptr(code);
            index = 0;
            *bits_left = 0;
        }
        if (*bits_left == 0) {
            char curr_char = string_to_char(byte_buffer);
            bytes_vector_add_byte(compression_vec, curr_char);
            bytes_vector_release(byte_buffer);
            bytes_vector_init(byte_buffer);
            *bits_left = 8;
        }
    }
}

int execute_compression(FILE** file) {
    huffman_compression_t huffman;
    if (huffman_compression_init(&huffman) != 0) return -3;//Memory Error

    bytes_vector_t compression_vec;
    if (bytes_vector_init(&compression_vec) != 0) {
        huffman_compression_release(&huffman);
        return -3;
    }

    bytes_vector_t byte_buffer;
    if (bytes_vector_init(&byte_buffer) != 0) {
        huffman_compression_release(&huffman);
        return -3;
    }

    char bits_left = 8;//Uso char porque int es demasiado grande
    char c = (char)getc(*file);
    bytes_vector_t* code;

    while (!feof(*file)) {
        code = huffman_compression_encode(&huffman, c);//Esto me esta devolviendo un bytes_vector de tamaño 320
        if (!code) return -4;//Letra inexistente
        process_code(&bits_left, &byte_buffer, code, &compression_vec);
        c = (char)getc(*file);
        bytes_vector_release(code);
        free(code);
    }

    //padding;
    for (int i = 0; i < bits_left; ++i) {
        bytes_vector_add_byte(&byte_buffer, '0');
    }
    char curr_char = string_to_char(&byte_buffer);
    bytes_vector_add_byte(&compression_vec, curr_char);

    //Toda esta parte cambiarla por una funcion en bytes vector que me deje cambiar un solo byte
    bytes_vector_t aux;
    bytes_vector_init(&aux);
    const char* vec_ptr = bytes_vector_get_ptr(&compression_vec);
    bytes_vector_add_byte(&aux, bits_left);
    for (int i = 0; i < bytes_vector_len(&compression_vec); ++i) {
        bytes_vector_add_byte(&aux, *(vec_ptr + i));
    }

    FILE* compressed = fopen("compressed.huffman", "w");
    vec_ptr = bytes_vector_get_ptr(&aux);
    fwrite(vec_ptr, sizeof(char), bytes_vector_len(&aux), compressed);
    fclose(compressed);

    //write_to_file();
    bytes_vector_release(&aux);
    bytes_vector_release(&byte_buffer);
    huffman_compression_release(&huffman);
    bytes_vector_release(&compression_vec);
}

int compress_file(const char* file_name) {
    if(!valid_file_extension(file_name)) return -1;//Invalid Extension

    FILE* file_to_compress = fopen(file_name, "r");
    if (!file_to_compress) return -2;//Could not open file

    execute_compression(&file_to_compress);

    fclose(file_to_compress);//Esto se podria comentar para mostrar el leak

    printf("listo");
    return 0;
}