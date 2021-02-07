// 15650 N과 M (2)

use std::fmt;
use std::io;

struct CTreeNode {
    num: u8,
    visited: bool,
    children: Vec<CTreeNode>,
}

impl CTreeNode {
    fn new(num: u8) -> Self {
        Self {
            num,
            visited: false,
            children: Vec::new(),
        }
    }

    fn populate(&mut self, nums: &Vec<u8>) {
        self.children = nums.iter().map(|&num| Self::new(num)).collect();
        self.children
            .iter_mut()
            .enumerate()
            .for_each(|(idx, child)| {
                let nums_greater: Vec<u8> = nums.iter().skip(idx + 1).copied().collect();
                if !nums_greater.is_empty() {
                    child.populate(&nums_greater);
                }
            });
    }
}

impl fmt::Display for CTreeNode {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> Result<(), fmt::Error> {
        write!(f, "{}", self.num)
    }
}

// combination tree
struct CTree {
    root: CTreeNode,
}

impl CTree {
    fn new(nums: &Vec<u8>) -> Self {
        let mut root = CTreeNode::new(0);
        root.populate(nums);
        Self { root }
    }

    fn print_until(&mut self, height: usize) {
        let tree_width = self.root.children.len();
        let num_of_combinations = (tree_width - height + 1..=tree_width).product::<usize>()
            / (1..=height).product::<usize>();
        let mut outputs = Vec::with_capacity(num_of_combinations);
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

    let mut tree = CTree::new(&nums);
    tree.print_until(m);
}
