use crate::huffman::huffman_tree::HuffmanTree;

mod huffman_node;
mod nodes_list;
mod huffman_tree;

pub struct HuffmanCompression {
    pub tree: HuffmanTree
}

impl HuffmanCompression {
    pub fn new() -> HuffmanCompression {
        return HuffmanCompression {
            tree: HuffmanTree::new()
        };
    }
}