pub struct HuffmanNode {
    letter: char,
    weight: i32,
    left: Option<Box<HuffmanNode>>,
    right: Option<Box<HuffmanNode>>,
}

impl HuffmanNode {
    pub fn new_leave(letter: char, frequency: i32) -> Box<HuffmanNode> {
        let node = HuffmanNode {
            letter,
            weight: frequency,
            left: None,
            right: None,
        };
        return Box::new(node);
    }

    pub fn merge_nodes(node1: Box<HuffmanNode>, node2: Box<HuffmanNode>) -> Box<HuffmanNode> {
        let w1 = node1.weight;
        let w2 = node2.weight;
        let left;
        let right;

        // lower to left, higher to right
        if w1 < w2 {
            left = node1;
            right = node2;
        } else {
            left = node2;
            right = node1;
        }

        let new_node = HuffmanNode {
            letter: char::from(0),
            weight: w1 + w2,
            left: Some(left),
            right: Some(right),
        };

        return Box::new(new_node);
    }

    pub fn is_leave(&self) -> bool {
        return match (&self.left, &self.right) {
            (None, None) => { true }
            (_, _) => { false }
        };
    }

    pub fn weight(&self) -> i32 {
        return self.weight;
    }

    pub fn print(&self) {
        if self.is_leave() {
            println!("leave: '{}' -> {}", self.letter.escape_default(), self.weight);
        } else {
            println!("not leave: {}", self.weight);
            match &self.left {
                None => {}
                Some(node) => { node.print() }
            }
            match &self.right {
                None => {}
                Some(node) => { node.print() }
            }
        }
    }
}
