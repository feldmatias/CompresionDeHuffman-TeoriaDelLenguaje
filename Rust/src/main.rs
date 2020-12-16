use crate::huffman::HuffmanCompression;
use std::env;

mod huffman;
mod decompression;

const PROGRAM_MODE_INDEX: usize = 1;
const FILE_INDEX: usize = 2;
const ARGS_AMOUNT: usize = 3;
const DECOMPRESS_ARG: &str = "decompress";
const COMPRESS_ARG: &str = "compress";

fn main() {
    /*
    let args: Vec<String> = env::args().collect();
    if args.len() != ARGS_AMOUNT {
        panic!("Incorrect amount of arguments");
    }
    if args[PROGRAM_MODE_INDEX] == COMPRESS_ARG {
        //llamar funcion de decompresion
    } else if args[PROGRAM_MODE_INDEX] == DECOMPRESS_ARG {
        decompression::decompress_file(&args[FILE_INDEX]);
    } else {
        panic!("Incorrect argument");
    }
    */


    let huffman = HuffmanCompression::new();
    huffman.print_tree();

}
