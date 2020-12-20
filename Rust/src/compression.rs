use std::fs;
use std::io::{Write};
use crate::huffman::{HuffmanCompression};

const TEXT_EXTENSION: &str = ".txt";
const COMPRESSED_EXTENSION :&str =".huffman";

//If the extension is .txt returns true, else returns false
fn valid_file_extension(file_name: &String) -> bool {
    let extension =  (*file_name).find(TEXT_EXTENSION);
    return match extension {
        Some(_) => true,
        None => false,
    }

}

//Receives a string of 0s and 1s and converts it to u8
fn string_to_u8(code: &String) -> u8 {
   return code.as_bytes().iter()
       .fold((0, 128), |(acc, mul), &c| (acc + (mul * (1&c as u8)), mul/2)).0;
}

//Receives a binary code and adds it to the byte buffer. If the byte buffer is completed then it is
//added to the compression vector
fn process_code(code: &mut String, byte_string: &mut String, bits_left: &mut i8, compression_vec: &mut Vec<u8>) {
    let mut finished_char = false;
    let mut code_len = code.len() as i8;

    while !finished_char {
        if (*bits_left - code_len) >= 0 {
            *bits_left -= code_len;
            byte_string.push_str(code);
            finished_char = true;
        } else if (*bits_left - code_len) < 0 {
            let aux = code.get(0..*bits_left as usize).unwrap();
            byte_string.push_str(aux);
            *code = code.get(*bits_left as usize..code_len as usize).unwrap().to_string();
            code_len = code.len() as i8;
            *bits_left = 0;
        }
        if *bits_left == 0 {
            let curr_char = string_to_u8(&byte_string);
            compression_vec.push(curr_char);
            byte_string.clear();
            *bits_left = 8;
        }
    }
}

//Adds the neccesary amount of 0s as padding to the final byte of the file.
fn add_padding(bits_left: i8, byte_string : &mut String) -> u8{
    if (bits_left % 8) != 0 {
        for _i in 0..bits_left {
            byte_string.push_str("0");
        }
    }
    return string_to_u8(&byte_string);
}

//Reads a text file and compresses it using Huffman coding. The compressed file is saved with
//the extension .huffman
pub fn compress_file(file_name: &String) {
    if !valid_file_extension(file_name){
        panic!("Invalid File Extension");
    }
    let file_to_compress = fs::read_to_string(file_name).expect("File does not exist");
    let huff_tree = HuffmanCompression::new();
    let mut compression_vec : Vec<u8> = Vec::new();
    let mut byte_buffer = String::new();
    let mut bits_left :i8 = 8;

    compression_vec.push(0);//Placeholder byte for the padding which will be added at the end

    for c in file_to_compress.chars() {
        match huff_tree.encode(c) {
            Ok(mut char_code) => {
                process_code(&mut char_code, &mut byte_buffer, &mut bits_left, &mut compression_vec);
            },
            Err(_) => {
                panic!("Inexistent character");
            }
        }
    }
    
    let padding = add_padding(bits_left, &mut byte_buffer);
    compression_vec.push(padding);

    let mut file = fs::File::create(file_name
                                .replace(TEXT_EXTENSION, COMPRESSED_EXTENSION))
                                .expect("Could not create file");
    compression_vec[0] = (bits_left % 8) as u8;//Adds the number of padding bits at the beginning of the file
    file.write_all(&*compression_vec).expect("Could not write file");
}
