use std::fs::File;
use std::io::{Write};


//Esta funcion la use para ir probando, despues tengo que usar la que hizo Mark
fn encode(c: char) -> &'static str {
    if c == 'H' {
        "1000001001100001"
    } else if c == 'u' {
        "10101"
    } else if c == 'f' {
        "01"
    } else {
        "000"
    }
}

fn string_to_u8(code: &String) -> u8 {
   return code.as_bytes().iter()
       .fold((0,128),|(acc,mul),&c| (acc+(mul*(1&c as u8)),mul/2)).0;
}

fn main() {
    let text = String::from("Huff");//Esto va a ser el archivo que leo
    let mut compression_vec : Vec<u8> = Vec::new();
    let mut byte_string = String::new();
    let mut char_code;
    let mut aux;
    let mut code_len : i8;
    let mut bits_left :i8 = 8;
    let mut finished_char;
    let mut curr_char;

    compression_vec.push(0);//para depsues meter el padding

    for c in text.chars() {
        finished_char = false;
        char_code = encode(c);
        code_len = char_code.len() as i8;

        while !finished_char {
            if (bits_left - code_len) >= 0 {
                bits_left -= code_len;
                byte_string.push_str(char_code);
                finished_char = true;
            } else if (bits_left - code_len) < 0 {
                aux = char_code.get(0..bits_left as usize).expect("Invalid Code");
                byte_string.push_str(aux);
                char_code = char_code.get(bits_left as usize..char_code.len()).expect("Invalid Code");
                code_len = char_code.len() as i8;
                bits_left = 0;
            }

            if bits_left == 0{
                curr_char = string_to_u8(&byte_string);
                compression_vec.push(curr_char);
                byte_string.clear();
                bits_left = 8;
            }
        }
    }

    //Agrego padding, ver si hay una mejor forma de hacerlo
    if (bits_left % 8) != 0 {
        for _i in 0..bits_left { //Si bien bits_left no puede ser mayor a 8 le hago el modulo para que cuando sea 8 no agregue padding
            byte_string.push_str("0");
        }
        curr_char = string_to_u8(&byte_string);
        compression_vec.push(curr_char);
    }

    //Creacion del archivo comprimido. Falta tomar el nombre del archivo que estoy leyendo en lugar de "test"
    // y sacar los prints etc
    let mut file = File::create("test.huffman").expect("Could not create file");
    compression_vec[0] = (bits_left % 8) as u8;
    file.write_all(&*compression_vec).expect("Could not write");
    let string_file= std::fs::read("test.huffman").expect("Something went wrong reading the file");
    println!("File");
    for c in string_file {
        println!("{}", c as u8);
    }
}
