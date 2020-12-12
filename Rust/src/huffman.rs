use crate::huffman::huffman_tree::HuffmanTree;

mod huffman_node;
mod nodes_list;
mod huffman_tree;
mod huffman_node_option2;

pub struct HuffmanCompression {
    pub tree: HuffmanTree
}

impl HuffmanCompression {
    pub fn new() -> HuffmanCompression {
        return HuffmanCompression {
            tree: HuffmanTree::new()
        };
    }

    pub fn print_tree(&self) {
        self.tree.print();
    }
}