use crate::huffman::HuffmanCompression;

mod huffman;

fn main() {
    let huffman = HuffmanCompression::new();
    huffman.print_tree();
}
