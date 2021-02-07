// 9663 N-Queen

use std::io;

#[derive(Clone)]
struct Point {
    x: i8,
    y: i8,
}

impl Point {
    fn new(x: i8, y: i8) -> Self {
        Self { x, y }
    }
}

struct NQTreeNode {
    queen: Point,
    children: Vec<NQTreeNode>,
}

impl NQTreeNode {
    fn new(queen: Point) -> Self {
        Self {
            queen,
            children: Vec::new(),
        }
    }

    // temporarily creates children nodes and count # of possible placements using DFS
    // child nodes are destroyed if and when the entire placements are evaluated
    fn count_placements(&mut self, safe_points: &Vec<Point>, repeat: usize) -> u32 {
        // try placing the next queen only on the next column
        // this reduces the # of cases to evaluate
        self.children = safe_points
            .iter()
            .filter(|point| point.x == self.queen.x + 1)
            .map(|point| Self::new(point.clone()))
            .collect();

        if repeat > 1 {
            let mut placements = 0;
            self.children.drain(..).for_each(|mut child| {
                let safe_points_after: Vec<Point> = safe_points
                    .iter()
                    .filter(|point| {
                        let tile = &child.queen;
                        // includes tile.x != point.x
                        // this is to eliminate duplicate placements
                        // by placing the queens in increasing x order
                        tile.x < point.x
                            && tile.y != point.y
                            && tile.x + tile.y != point.x + point.y
                            && point.x - tile.x != point.y - tile.y
                    })
                    .cloned()
                    .collect();
                if !safe_points_after.is_empty() {
                    placements += child.count_placements(&safe_points_after, repeat - 1);
                }
            });
            placements
        } else {
            self.children.len() as u32
        }
    }
}

fn count_nqueen(n: u8) -> u32 {
    let mut root = NQTreeNode::new(Point::new(-1, -1));
    let board: Vec<Point> = (0..n as i8)
        .flat_map(|x| (0..n as i8).map(move |y| Point::new(x, y)))
        .collect();
    root.count_placements(&board, n as usize)
}

fn main() {
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");
    let n: u8 = input.trim().parse().unwrap();
    println!("{}", count_nqueen(n));
}
