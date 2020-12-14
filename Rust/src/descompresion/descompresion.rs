//use std::fs;
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut i = 0;
    for arg in args.iter() {
        println!("Argumento {0}: {1}", i, arg);
        i += 1;
    }
    //println!("{:?}", args);
}