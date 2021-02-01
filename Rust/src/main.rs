#![allow(warnings, unused)]

use crate::huffman::HuffmanCompression;
use std::env;

mod huffman;
mod decompression;
mod compression;

const PROGRAM_MODE_INDEX: usize = 1;
const FILE_INDEX: usize = 2;
const ARGS_AMOUNT: usize = 3;
const PRINT_TREE_ARGS_AMOUNT: usize = 2;
const DECOMPRESS_ARG: &str = "decompress";
const COMPRESS_ARG: &str = "compress";
const PRINT_TREE: &str = "print";
const INCORRECT_ARGUMENTS_MESSAGE: &str = "Incorrect Argument: valid arguments are \n\
                                           1) print [see the tree nodes and char frequencies]\n\
                                           2) compress filename [compress the file named filename]\n\
                                           3) decompress filename [decompress the file named filename]\n";

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() == PRINT_TREE_ARGS_AMOUNT {
        if args[PROGRAM_MODE_INDEX] == PRINT_TREE {
            let huffman = HuffmanCompression::new();
            huffman.print_tree();
        } else {
            panic!("{}", INCORRECT_ARGUMENTS_MESSAGE);
        }
    } else if args.len() == ARGS_AMOUNT {
        if args[PROGRAM_MODE_INDEX] == COMPRESS_ARG {
            compression::compress_file(&args[FILE_INDEX]);
        } else if args[PROGRAM_MODE_INDEX] == DECOMPRESS_ARG {
            decompression::decompress_file(&args[FILE_INDEX]);
        } else {
            panic!("{}", INCORRECT_ARGUMENTS_MESSAGE);
        }
    } else {
        println!("{}", INCORRECT_ARGUMENTS_MESSAGE);
    }
}
