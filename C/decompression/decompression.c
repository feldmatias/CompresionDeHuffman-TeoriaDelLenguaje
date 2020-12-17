#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "decompression.h"
#include "../huffman/huffman_tree.h"
#include "bytes_vector/bytes_vector.h"

#define COMPRESSED_EXTENSION ".huffman"
#define DECOMPRESSED_EXTENSION ".txt"
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
fn get_char(bytes: &Vec<u8>, huff_tree: &HuffmanCompression, byte_to_read: &mut usize, read_bits: u8,
decompressed_file: &mut String) -> u8 {
let mut aux_byte: u8;
let mut was_letter_decoded = false;
let mut tree_code: String = String::from("");
let mut _read_bits: u8 = read_bits;

while !was_letter_decoded {
//Gets the corresponding bit
aux_byte = (bytes[*byte_to_read] << read_bits) >> (read_bits + BITS_PER_BYTE - (read_bits + 1));

tree_code.push(std::char::from_u32(aux_byte as u32).expect("Invalid number for tree code"));
_read_bits += 1;
match huff_tree.decode(&tree_code) {
Some(letter) => {
decompressed_file.push(letter);
was_letter_decoded = true;
},
None => {},
}
if _read_bits == BITS_PER_BYTE {
_read_bits = 0;
(*byte_to_read) += 1;
}
}
return _read_bits;
}

char get_char(const char* compressed_file_string, huff_tree: &HuffmanCompression, int* byte_to_read, char read_bits,
              bytes_vector_t* decompressed_file) -> u8 {

}
int execute_decompression(const char* file_name, const char* compressed_file_string, int compressed_file_len) {
    char padding_bits = compressed_file_string[0];
    int byte_to_read = 1;
    int read_bits = 0;
    char last_byte_read_bits = 0;

    //CREAR EL HUFF_TREE

    bytes_vector_t decompressed_file_text;
    if (bytes_vector_init(&decompressed_file_text) != SUCCESS) {
        return MEMORY_ERROR;
    }

    while (byte_to_read < compressed_file_len) {
        read_bits = get_char(&compressed_file, &mut byte_to_read, read_bits,
                &mut decompressed_file_text);
        if byte_to_read == last_byte_to_read {
            last_byte_read_bits += read_bits;
        } else {
            last_byte_read_bits = 0;
        }
        last_byte_to_read = byte_to_read;
        if byte_to_read == (compressed_file.len() - 1) {
            if (BITS_PER_BYTE - last_byte_read_bits) == padding_bits {
                byte_to_read += 1;
            }
        }
    }

    if (bytes_vector_release(&decompressed_file_text) != SUCCESS) {
        return MEMORY_ERROR;
    }

    //DESTRUIR EL HUFFMAN TREE

    return SUCCESS;
}



int decompress_file(const char* file_name) {
    if (!is_file_name_valid(file_name)) {
        return BAD_FILE_NAME;
    }

    //Hacer que el struct este en el .h para que se pueda guardar en el stack
    //huffman_compression_t tree

    huffman_tree_decode();
    char* compressed_file_string;
    int file_len = 0;
    int program_status = load_file(file_name, &compressed_file_string, &file_len);
    if (program_status != SUCCESS) {
        free(compressed_file_string);
        return program_status;
    }

    //llamar a execute_decompression
    program_status = execute_decompression(file_name, compressed_file_string, file_len);


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