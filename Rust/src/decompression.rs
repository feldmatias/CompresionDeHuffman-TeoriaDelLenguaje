use std::fs;
use std::io::prelude::*;
use crate::huffman::HuffmanCompression;

const BITS_PER_BYTE: u8 = 8;
const COMPRESSED_EXTENSION: &str = ".huffman";
const DECOMPRESSED_EXTENSION: &str = ".txt";

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
        println!("Aux Byte {}", std::char::from_u32(aux_byte as u32 + '0' as u32).expect("Invalid number for tree code"));
        tree_code.push(std::char::from_u32(aux_byte as u32 + '0' as u32).expect("Invalid number for tree code"));
        _read_bits += 1;
        match huff_tree.decode(&tree_code) {
            Some(letter) => {
                /*println!("Letra: {}", letter);
                for c in tree_code.chars(){
                    print!("{}", c);
                }
                println!();*/

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

//Checks if the extension is at the end of the name of the file
fn has_valid_file_name(file_name: &String) -> bool {
    return (*file_name).find(COMPRESSED_EXTENSION).expect("Unknown extension") +
           COMPRESSED_EXTENSION.len() == file_name.len();
}

fn execute_decompression(file_name: &String, decompressed_file_text: &mut String) {
    let compressed_file = fs::read(file_name).expect("Inexistent file");
    let padding_bits = compressed_file[0];
    let mut byte_to_read = 1;//byte que se esta procesando
    let mut last_byte_to_read = 1;//byte que se proceso en la iteracion anterior
    let mut read_bits = 0;
    let mut last_byte_read_bits = 0;//Cantidad de bits totales leidos del byte que se
                                        //esta procesando
    let huff_tree = HuffmanCompression::new();
    while byte_to_read < compressed_file.len() {
        read_bits = get_char(&compressed_file, &huff_tree, &mut byte_to_read, read_bits,
                             decompressed_file_text);
        if byte_to_read == last_byte_to_read {
            last_byte_read_bits += read_bits;
        } else {
            last_byte_read_bits = read_bits;
        }
        last_byte_to_read = byte_to_read;
        if byte_to_read == (compressed_file.len() - 1) {
            if (BITS_PER_BYTE - last_byte_read_bits) == padding_bits {
                byte_to_read += 1;
            }
        }
    }

}

//Receives a huffman compressed text and creates a decompressed .txt version
//The name of the file must contain only contain one ".", located in ".huffman"
//The filename must end with .huffman, otherwise the program will panic
pub fn decompress_file(file_name: &String) {
    if !has_valid_file_name(file_name) {
        panic!("Invalid file name");
    }
    let mut decompressed_file_text = String::from("");
    execute_decompression(file_name, &mut decompressed_file_text);

    //CAMBIAR ESTO PORQUE PUEDO HACER QUE LA FUNCION QUE CHEQUEA EL NOMBRE DEL ARCHIVO RETORNE LA POSICION
    //DE LA EXTENSION, TENDRIA QUE SACAR EL EXPECT DEL find
    let mut decompressed_file = fs::File::create(file_name[..file_name.find(COMPRESSED_EXTENSION).
                                                      expect("Wrong extension")].to_string() + DECOMPRESSED_EXTENSION).
                                                      expect("Problem creating the file");
    decompressed_file.write_all( decompressed_file_text.as_bytes()).expect("Problem writing to file");
}
