// 2042 구간 합 구하기

use std::collections::VecDeque;
use std::convert::From;
use std::io;
use std::io::prelude::*;
use std::ops::Range;
use std::str::FromStr;

struct UpdateQuery {
    target: usize,
    with: i64,
}

impl UpdateQuery {
    fn new(target: usize, with: i64) -> Self {
        Self { target, with }
    }
}

struct ReduceQuery {
    range: Range<usize>,
}

impl ReduceQuery {
    fn new(left: usize, right: usize) -> Self {
        Self {
            range: (left..right),
        }
    }
}

enum Query {
    Update(UpdateQuery),
    Reduce(ReduceQuery),
}

impl FromStr for Query {
    type Err = ();

    fn from_str(line: &str) -> Result<Self, Self::Err> {
        let params: Vec<&str> = line.split_whitespace().collect();
        match params[0] {
            "1" => {
                let target: usize = params[1].parse().unwrap();
                let with: i64 = params[2].parse().unwrap();
                Ok(Self::Update(UpdateQuery::new(target - 1, with)))
            }
            "2" => {
                let left: usize = params[1].parse().unwrap();
                let right: usize = params[2].parse().unwrap();
                Ok(Self::Reduce(ReduceQuery::new(left - 1, right)))
            }
            _ => Err(()),
        }
    }
}

struct SegmentNode {
    range: Range<usize>,
    sum: i64,
    left: Option<Box<SegmentNode>>,
    right: Option<Box<SegmentNode>>,
}

impl SegmentNode {
    fn new(index: usize, num: i64) -> Self {
        Self {
            range: (index..index + 1),
            sum: num,
            left: None,
            right: None,
        }
    }

    fn merge(left: Self, right: Self) -> Self {
        Self {
            range: (left.range.start..right.range.end),
            sum: left.sum + right.sum,
            left: Some(Box::from(left)),
            right: Some(Box::from(right)),
        }
    }
}

struct SegmentTree {
    root: SegmentNode,
}

impl SegmentTree {
    fn compute(&mut self, query: Query) -> Option<i64> {
        match &query {
            Query::Update(update) => {
                fn recalc(node: &mut SegmentNode, target: usize, with: i64) {
                    if node.left.is_none() {
                        node.sum = with;
                    } else {
                        if node.left.as_ref().unwrap().range.contains(&target) {
                            recalc(node.left.as_mut().unwrap(), target, with);
                        } else {
                            recalc(node.right.as_mut().unwrap(), target, with);
                        }
                        node.sum =
                            node.left.as_ref().unwrap().sum + node.right.as_ref().unwrap().sum;
                    }
                }
                recalc(&mut self.root, update.target, update.with);
                None
            }
            Query::Reduce(reduce) => {
                fn sum_range(node: &mut SegmentNode, query_range: Range<usize>) -> i64 {
                    let left_ref = node.left.as_ref().unwrap();
                    let right_ref = node.right.as_ref().unwrap();

                    if node.range.start == query_range.start && node.range.end == query_range.end {
                        // query range is of exact match with this node
                        node.sum
                    } else if query_range.end <= left_ref.range.end {
                        // query range is entirely included in left node
                        sum_range(node.left.as_mut().unwrap(), query_range)
                    } else if right_ref.range.start <= query_range.start {
                        // query range is entirely included in right node
                        sum_range(node.right.as_mut().unwrap(), query_range)
                    } else {
                        // query range is across both nodes
                        let left_range = query_range.start..left_ref.range.end;
                        let right_range = right_ref.range.start..query_range.end;
                        sum_range(node.left.as_mut().unwrap(), left_range)
                            + sum_range(node.right.as_mut().unwrap(), right_range)
                    }
                }
                Some(sum_range(&mut self.root, reduce.range.clone()))
            }
        }
    }
}

impl From<Vec<i64>> for SegmentTree {
    fn from(nums: Vec<i64>) -> Self {
        // conversion involves pop_front, so use VecDeque for performance
        let mut nodes: VecDeque<SegmentNode> = nums
            .iter()
            .enumerate()
            .map(|(index, &num)| SegmentNode::new(index, num))
            .collect();
        loop {
            let mut merged_nodes = VecDeque::with_capacity(nodes.len() / 2 + 1);
            while nodes.len() > 1 {
                let left = nodes.pop_front().unwrap();
                let right = nodes.pop_front().unwrap();
                let parent = SegmentNode::merge(left, right);
                merged_nodes.push_back(parent);
            }
            merged_nodes.append(&mut nodes);
            if merged_nodes.len() == 1 {
                return Self {
                    root: merged_nodes.pop_front().unwrap(),
                };
            } else {
                nodes = merged_nodes;
            }
        }
    }
}

fn main() {
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");
    let tree_len: usize = input
        .trim()
        .split_whitespace()
        .nth(0)
        .unwrap()
        .parse()
        .unwrap();

    let nums: Vec<i64> = io::stdin()
        .lock()
        .lines()
        .take(tree_len)
        .map(|line| line.unwrap().parse().unwrap())
        .collect();
    let mut tree = SegmentTree::from(nums);

    print!(
        "{}",
        io::stdin()
            .lock()
            .lines()
            .map(|line| Query::from_str(&line.unwrap()).unwrap())
            .filter_map(|query| tree.compute(query))
            .map(|partial_sum| format!("{}\n", partial_sum))
            .collect::<String>()
    );
}
