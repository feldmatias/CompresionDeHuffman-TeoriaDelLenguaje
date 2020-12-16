pub struct HuffmanNode {
    letter: char,
    weight: i32,
    left: Option<Box<HuffmanNode>>,
    right: Option<Box<HuffmanNode>>,
}

pub struct BadPathError;

impl HuffmanNode {

    pub fn new_leaf(letter: char, frequency: i32) -> Box<HuffmanNode> {
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

    pub fn is_leaf(&self) -> bool {
        return match (&self.left, &self.right) {
            (None, None) => { true }
            (_, _) => { false }
        };
    }

    /*
     * Given a code, it returns it's associated letter. If the code corresponds to a non-leaf node
     * (therefore it does not contain a letter) then it returns None.
     */
    pub fn get_letter(&self, tree_code_it: &mut std::str::Chars) -> Option<char> {
        if self.is_leaf() {
            Some(self.letter)
        } else {
            let it = tree_code_it.next();
            if it? == '0' {
                self.left.as_ref().unwrap().get_letter(tree_code_it) //unwrap will never panic because we know for a fact there is a child since we are not a leaf node
            } else { //'1'
                self.right.as_ref().unwrap().get_letter(tree_code_it)
            }
        }
    }

    /*
     * Recursively travels the tree in a CLR (center-left-right) manner until it finds (if it exists)
     * the node with the given letter, returning the code associated with the tree path. If the letter
     * is not present in any node, then it returns an InexistentLetterError.
     */
    pub fn get_code(&self, letter: char) -> Result<String, BadPathError> {
        return if self.letter == letter {
            Ok(String::new())
        } else if self.is_leaf() {
            Err(BadPathError)
        } else {
            let left_code = self.left.as_ref().unwrap().get_code(letter); //it will never panic because we are not a leaf node
            return match left_code {
                Err(_) => {
                    let mut right_code = self.right.as_ref().unwrap().get_code(letter)?;
                    right_code.push('1');
                    Ok(right_code)
                },
                Ok(mut c) => {
                    c.push('0');
                    Ok(c)
                },
            }
        }
    }

    pub fn weight(&self) -> i32 {
        return self.weight;
    }

    pub fn print(&self) {
        if self.is_leaf() {
            println!("leaf: '{}' -> {}", self.letter.escape_default(), self.weight);
        } else {
            println!("not leaf: {}", self.weight);
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
