#![allow(warnings, unused)]

use crate::huffman::huffman_tree::HuffmanTree;

mod huffman_node;
mod nodes_list;
mod huffman_tree;
mod huffman_node_option2;

pub struct InexistentLetterError;

pub struct HuffmanCompression {
    pub tree: HuffmanTree,
}

impl HuffmanCompression {
    pub fn new() -> HuffmanCompression {
        return HuffmanCompression {
            tree: HuffmanTree::new()
        };
    }

    /*
     * Given a letter code, returns the corresponded letter. If the code is not associated
     * with a letter then it returns None.
     */
    pub fn decode(&self, tree_code: &String) -> Option<char> {
        self.tree.get_letter(tree_code)
    }

    /*
     * Given a letter, returns the corresponded code. If the letter does not exist it returns
     * an InexistentLetterError.
     */
    pub fn encode(&self, letter: char) -> Result<String, InexistentLetterError> {
        self.tree.get_code(letter)
    }

    pub fn print_tree(&self) {
        self.tree.print();
    }
}
