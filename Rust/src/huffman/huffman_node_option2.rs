#![allow(dead_code)]

use std::cell::RefCell;
use std::rc::Rc;


/*

This is another version of the Huffman Node which has a reference to the parent,
allowing to walk the tree starting from the leaves, making it more performant than brute force.

However, this method has a clear disadvantage. Since we need to store references to the same node in multiple places,
and we need them to be mutable because we need to update the parent of the nodes,
the only way to achieve it is with RefCell (A mutable memory location with dynamically checked borrow rules).
This means that we are losing the security of the compiler checks.
This is because we can't have multiple mutable references of the same object at the same time.

This has other disadvantages, because we are wrapping the nodes in Rc and RefCell, to access the node
we need to do something like this: `node.as_ref().borrow().weight;` every time,
losing readability in the code.

*/

pub struct HuffmanNode2 {
    letter: char,
    weight: i32,
    left: Option<Rc<RefCell<HuffmanNode2>>>,
    right: Option<Rc<RefCell<HuffmanNode2>>>,
    parent: Option<Rc<RefCell<HuffmanNode2>>>, // This allows to walk the tree upwards.
}

impl HuffmanNode2 {
    pub fn weight(&self) -> i32 {
        return self.weight;
    }

    pub fn new_leave(letter: char, frequency: i32) -> Rc<RefCell<HuffmanNode2>> {
        let node = HuffmanNode2 {
            letter: letter,
            weight: frequency,
            left: None,
            right: None,
            parent: None,
        };
        return Rc::new(RefCell::new(node));
    }

    pub fn merge_nodes(node1: &Rc<RefCell<HuffmanNode2>>, node2: &Rc<RefCell<HuffmanNode2>>) -> Rc<RefCell<HuffmanNode2>> {
        let w1 = node1.as_ref().borrow().weight;
        let w2 = node2.as_ref().borrow().weight;
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

        let new_node = Rc::new(RefCell::new(HuffmanNode2 {
            letter: char::from(0),
            weight: w1 + w2,
            left: Some(left.clone()),
            right: Some(right.clone()),
            parent: None,
        }));

        // Here is why we need mutable references. We need to set the parent for the child nodes.
        node1.borrow_mut().parent = Some(new_node.clone());
        node2.borrow_mut().parent = Some(new_node.clone());
        return new_node;
    }

    pub fn is_leave(&self) -> bool {
        return match (&self.left, &self.right) {
            (None, None) => { true }
            (_, _) => { false }
        };
    }

    pub fn is_root(&self) -> bool {
        return match &self.parent {
            None => { true }
            Some(_) => { false }
        };
    }

    pub fn print(&self) {
        let parent = match &self.parent {
            None => { 0 }
            Some(p) => { p.as_ref().borrow().weight }
        };

        if self.is_leave() {
            println!("leave: '{}' -> {} (parent: {})", self.letter.escape_default(), self.weight, parent);
        } else {
            println!("not leave: {}  (parent: {})", self.weight, parent);
            match &self.left {
                None => {}
                Some(n) => { n.as_ref().borrow().print() }
            }
            match &self.right {
                None => {}
                Some(n) => { n.as_ref().borrow().print() }
            }
        }
    }
}