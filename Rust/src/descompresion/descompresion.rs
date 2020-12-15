use std::fs;
use std::env;

const FILE_INDEX: usize = 2;
const ARGS_AMOUNT: usize = 3;
const BITS_PER_BYTE: usize = 8;

//Returns the number of read bits
fn get_char(bytes: &Vec<u8>, byte_to_read: &mut usize, read_bits: usize, decompressed_file: &mut String) -> u8 {
    //Leaves the already read bits in 0
    //let mut aux_byte: u8 = (bytes[byte_to_read] << read_bits) >> read_bits;

    let was_letter_decoded = false;
    let mut tree_code: String;
    let mut _read_bits = read_bits;

    while !was_letter_decoded {
        //Gets the corresponding bit
        aux_byte = (bytes[byte_to_read] << read_bits) >> (read_bits + BITS_PER_BYTE - (read_bits + 1));

        tree_code.push(std::char::from_u32(aux_byte).expect("Invalid number for tree code"));
        _read_bits += 1;
        
    }

    //CAMBIAR EL RETURN
    return 0;
}



fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != ARGS_AMOUNT {
        panic!("Incorrect amount of arguments");
    }
    let compressed_file = fs::read(&args[FILE_INDEX]).expect("Inexistent file");
    let padding_bits = compressed_file[0];
    let mut byte_to_read = 1;
    while byte_to_read < compressed_file.len() {
        
    }
}
