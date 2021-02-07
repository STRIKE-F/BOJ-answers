// 15652 N과 M (4)

use std::fmt;
use std::io;

struct NDSTreeNode {
    num: u8,
    visited: bool,
    children: Vec<NDSTreeNode>,
}

impl NDSTreeNode {
    fn new(num: u8) -> Self {
        Self {
            num,
            visited: false,
            children: Vec::new(),
        }
    }

    fn populate(&mut self, nums: &Vec<u8>, repeat: usize) {
        self.children = nums.iter().map(|&num| Self::new(num)).collect();
        // should population be repeated
        if repeat > 1 {
            self.children
                .iter_mut()
                .enumerate()
                .for_each(|(idx, child)| {
                    let nums_greater: Vec<u8> = nums.iter().skip(idx).copied().collect();
                    if !nums_greater.is_empty() {
                        child.populate(&nums_greater, repeat - 1);
                    }
                });
        }
    }
}

impl fmt::Display for NDSTreeNode {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> Result<(), fmt::Error> {
        write!(f, "{}", self.num)
    }
}

// nondecreasing selection tree
struct NDSTree {
    root: NDSTreeNode,
}

impl NDSTree {
    fn new(nums: &Vec<u8>) -> Self {
        let mut root = NDSTreeNode::new(0);
        root.populate(nums, nums.len());
        Self { root }
    }

    fn print_until(&mut self, height: usize) {
        let tree_width = self.root.children.len();
        // don't know the exact number, so reserve enough space
        let num_of_selections = tree_width.pow(tree_width as u32);
        let mut outputs = Vec::with_capacity(num_of_selections);
        let mut trace = Vec::with_capacity(height);

        // root is visited (only) after the entire tree has been traversed
        while !self.root.visited {
            trace.clear();
            let mut cur = &mut self.root;
            for _ in 0..height {
                if let Some(new_cur) = cur.children.iter_mut().find(|child| !child.visited) {
                    cur = new_cur;
                    trace.push(cur.num.to_string());
                } else {
                    cur.visited = true;
                    break;
                }
            }

            // if tree has successfully DFSed all the way into target height
            if trace.len() == height {
                cur.visited = true;
                outputs.push(trace.join(" "));
            }
        }
        println!("{}", outputs.join("\n"));
    }
}

fn main() {
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");
    let params: Vec<&str> = input.trim().split_whitespace().collect();
    let nums: Vec<u8> = (1..=params[0].parse().unwrap()).collect();
    let m = params[1].parse().unwrap();

    let mut tree = NDSTree::new(&nums);
    tree.print_until(m);
}
