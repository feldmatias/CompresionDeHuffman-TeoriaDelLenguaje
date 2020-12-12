use crate::huffman::huffman_node::HuffmanNode;

// Helper class to store temporary nodes and easily get the ones with minimum weight
pub struct NodesList {
    nodes: Vec<Box<HuffmanNode>>
}

impl NodesList {
    pub fn new() -> NodesList {
        return NodesList {
            nodes: Vec::new()
        };
    }

    pub fn add_node(&mut self, node: Box<HuffmanNode>) {
        self.nodes.push(node);
    }

    pub fn get_min_node(&mut self) -> Box<HuffmanNode> {
        let mut min_index = 0;
        let mut min_weight = i32::max_value();

        for (index, node) in self.nodes.iter().enumerate() {
            if node.weight() < min_weight {
                min_index = index;
                min_weight = node.weight();
            }
        }

        let node = self.nodes.remove(min_index);
        return node;
    }

    pub fn length(&self) -> usize {
        return self.nodes.len();
    }
}