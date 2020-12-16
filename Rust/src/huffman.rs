use crate::huffman::huffman_tree::HuffmanTree;
//use crate::huffman::huffman_node::NonLetterError;

mod huffman_node;
mod nodes_list;
mod huffman_tree;
mod huffman_node_option2;

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
     * Given a letter, returns the corresponded code.
     */
    pub fn encode(&self, letter: char) -> String {
        return String::new()
    }

    pub fn print_tree(&self) {
        self.tree.print();
    }
}
