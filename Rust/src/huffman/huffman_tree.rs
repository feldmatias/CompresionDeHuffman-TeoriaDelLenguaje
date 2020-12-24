#![allow(warnings, unused)]

use crate::huffman::huffman_node::HuffmanNode;
use crate::huffman::nodes_list::NodesList;
use crate::huffman::InexistentLetterError;

const SIZE: usize = 100;

const CHARS: [char; SIZE] = [
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
    'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*',
    '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`',
    '{', '|', '}', '~', ' ', '\t', '\n', '\r', '\x0b', '\x0c',
];

const FREQUENCIES: [i32; SIZE] = [
    325, 982, 373, 343, 107, 94, 73, 45, 47, 966, 245509, 46768, 67194, 134216, 406157, 69103,
    57328, 218875, 199130, 2788, 29345, 146532, 95890, 216805, 282560, 46849, 2414, 209907,
    215605, 291243, 115225, 34077, 73155, 4681, 85549, 1098, 44560, 15450, 21526, 15726, 42703,
    11770, 11261, 18486, 55893, 2071, 6206, 23919, 15887, 27393, 33268, 12038, 1179, 29040, 34082,
    39878, 14169, 3587, 16508, 608, 9128, 532, 8840, 488, 3, 2, 1, 21, 31077, 643, 644, 73, 0,
    83315, 8150, 78216, 48, 1843, 17199, 468, 0, 440, 10476, 3, 2076, 0, 2068, 0, 71, 1, 0, 33,
    2, 1, 1285884, 0, 124787, 0, 0, 0,
];

pub struct HuffmanTree {
    root: Box<HuffmanNode>
}

impl HuffmanTree {
    pub fn new() -> HuffmanTree {
        let mut nodes_list = HuffmanTree::create_leaf_nodes();
        while nodes_list.length() > 1 {
            let node1 = nodes_list.get_min_node();
            let node2 = nodes_list.get_min_node();
            let merged_node = HuffmanNode::merge_nodes(node1, node2);
            nodes_list.add_node(merged_node);
        }

        let root = nodes_list.get_min_node(); // last node
        return HuffmanTree {
            root
        };
    }

    fn create_leaf_nodes() -> NodesList {
        let mut nodes_list = NodesList::new();
        for i in 0..SIZE {
            let leaf_node = HuffmanNode::new_leaf(CHARS[i], FREQUENCIES[i]);
            nodes_list.add_node(leaf_node);
        }
        return nodes_list;
    }

    /*
     * Given a code, it returns it's associated letter. If the code corresponds to a non-leaf node
     * (therefore it does not contain a letter) then it returns None.
     */
    pub fn get_letter(&self, tree_code: &String) -> Option<char> {
        self.root.get_letter(&mut tree_code.chars())
    }

    /*
     * Given a letter, returns the corresponded code. If the letter does not exist it returns
     * an InexistentLetterError.
     */
    pub fn get_code(&self, letter: char) -> Result<String, InexistentLetterError> {
        return match self.root.get_code(letter) {
            //Ok(code) => code.graphemes(true).rev().collect(),
            Ok(code) => Ok(code.chars().rev().collect::<String>()),
            Err(_) => Err(InexistentLetterError),
        }
    }

    pub fn print(&self) {
        println!("_____________");
        println!("Huffman Tree");
        println!("_____________");
        self.root.print();
        println!("_____________");
    }
}