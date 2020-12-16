use std::fs;
use std::env;
use std::io::prelude::*;
//use std::path::Path;
//use crate::huffman::huffman_tree::HuffmanTree;
use crate::huffman::HuffmanCompression;

//mod huffman::huffman_tree;

const FILE_INDEX: usize = 2;
const ARGS_AMOUNT: usize = 3;
const BITS_PER_BYTE: u8 = 8;

//Returns the number of read bits
fn get_char(bytes: &Vec<u8>, byte_to_read: &mut usize, read_bits: u8, decompressed_file: &mut String) -> u8 {
    //Leaves the already read bits in 0
    //let mut aux_byte: u8 = (bytes[byte_to_read] << read_bits) >> read_bits;

    let mut was_letter_decoded = false;
    let mut tree_code: String;
    let mut _read_bits: u8 = read_bits;

    while !was_letter_decoded {
        //Gets the corresponding bit
        aux_byte = (bytes[byte_to_read] << read_bits) >> (read_bits + BITS_PER_BYTE - (read_bits + 1));

        tree_code.push(std::char::from_u32(aux_byte).expect("Invalid number for tree code"));
        _read_bits += 1;
        match HuffmanCompression::decode(&tree_code) {
            Ok(letter) => {
                decompressed_file.push(letter);
                was_letter_decoded = True;
            },
        }
        if _read_bits == BITS_PER_BYTE {
            _read_bits = 0;
            (*byte_to_read) += 1;
        }
    }
    return _read_bits;
}



fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != ARGS_AMOUNT {
        panic!("Incorrect amount of arguments");
    }
    let compressed_file = fs::read(&args[FILE_INDEX]).expect("Inexistent file");
    let padding_bits = compressed_file[0];
    let mut byte_to_read = 1;
    let mut read_bits = 0;
    let mut decompressed_file_text;
    let mut last_byte_read_bits = 0;
    while byte_to_read < compressed_file.len() {
        read_bits = get_char(&compressed_file, &mut byte_to_read, read_bits,
                             &mut decompressed_file_text);
        if byte_to_read == (compressed_file.len() - 1) {
            last_byte_read_bits += read_bits;
            if (BITS_PER_BYTE - last_byte_read_bits) == padding_bits {
                byte_to_read += 1;
            }
        }
    }
    let mut decompressed_file = fs::File::create("foo.txt").expect("Problem creating the file");
    decompressed_file.write_all( decompressed_file_text.as_bytes()).expect("Problem writing to file");
}
