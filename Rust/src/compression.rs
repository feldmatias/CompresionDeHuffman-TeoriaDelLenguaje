use std::fs;
use std::io::{Write};
use crate::huffman::{HuffmanCompression, InexistentLetterError};

fn string_to_u8(code: &String) -> u8 {
   return code.as_bytes().iter()
       .fold((0,128),|(acc,mul),&c| (acc+(mul*(1&c as u8)),mul/2)).0;
}

fn process_code(code: &mut String, byte_string: &mut String, bits_left: &mut i8, compression_vec: &mut Vec<u8>) {
    let mut finished_char = false;
    let mut code_len = code.len() as i8;

    while !finished_char {
        if (*bits_left - code_len) >= 0 {
            *bits_left -= code_len;
            byte_string.push_str(code);
            finished_char = true;
        } else if (*bits_left - code_len) < 0 {
            let aux = code.get(0..*bits_left as usize).expect("Invalid Code");
            byte_string.push_str(aux);
            *code = code.get(*bits_left as usize..code_len as usize)
                                    .expect("Invalid Code").parse().unwrap();
            code_len = code.len() as i8;
            *bits_left = 0;
        }
        if *bits_left == 0{
            let curr_char = string_to_u8(&byte_string);
            compression_vec.push(curr_char);
            byte_string.clear();
            *bits_left = 8;
        }
    }
}

pub fn compress_file(file_name: &String) {
    //let file_to_compress = fs::read_to_string(file_name).expect("File does not exist");
    let file_to_compress = String::from("Huffman");
    let mut compression_vec : Vec<u8> = Vec::new();
    let mut byte_string = String::new();
    let mut bits_left :i8 = 8;

    let huff_tree = HuffmanCompression::new();

    compression_vec.push(0);//Aca voy a poner el padding al final

    for c in file_to_compress.chars() {
        match huff_tree.encode(c) {
            Ok(mut char_code) => {
                process_code(&mut char_code, &mut byte_string, &mut bits_left, &mut compression_vec);
            },
            Err(E) => {
                println!("Error:");
            }
        }
    }

    //Agrego padding, ver si hay una mejor forma de hacerlo
    if (bits_left % 8) != 0 {
        for _i in 0..bits_left { //Si bien bits_left no puede ser mayor a 8 le hago el modulo para que cuando sea 8 no agregue padding
            byte_string.push_str("0");
        }
        let curr_char = string_to_u8(&byte_string);
        compression_vec.push(curr_char);
    }

    //Creacion del archivo comprimido. Falta tomar el nombre del archivo que estoy leyendo en lugar de "test"
    // y sacar los prints etc
    let mut file = fs::File::create("test.huffman").expect("Could not create file");
    compression_vec[0] = (bits_left % 8) as u8;
    file.write_all(&*compression_vec).expect("Could not write");
    let string_file= std::fs::read("test.huffman").expect("Something went wrong reading the file");
    println!("File");
    for c in string_file {
        println!("{}", c as u8);
    }
}
