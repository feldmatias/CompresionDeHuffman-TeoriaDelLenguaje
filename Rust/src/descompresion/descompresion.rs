use std::fs;
use std::env;

const FILE_INDEX: usize = 2;
const ARGS_AMOUNT: usize = 3;

fn main() {
    let args: Vec<String> = env::args().collect();
    if (args.len() != ARGS_AMOUNT) {
        panic!("Incorrect amount of arguments");
    }
    let compressed_file = fs::read_to_string(&args[FILE_INDEX]).expect("Inexistent file");

}
