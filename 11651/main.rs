// 11651 좌표 정렬하기 2

use std::fmt::{Display, Error, Formatter};
use std::io;
use std::io::prelude::*;

#[derive(PartialOrd, Ord, PartialEq, Eq)]
struct Point {
    y: i32,
    x: i32,
}

impl Point {
    fn new(x: i32, y: i32) -> Self {
        Self { x, y }
    }
}

impl Display for Point {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), Error> {
        write!(f, "{} {}", self.x, self.y)
    }
}

fn main() {
    // # of coordinates is redundant
    io::stdin()
        .read_line(&mut String::new())
        .expect("readline failed: ");

    let mut points: Vec<Point> = io::stdin()
        .lock()
        .lines()
        .map(|line| {
            let coords: Vec<i32> = line
                .unwrap()
                .split_whitespace()
                .map(|word| word.parse().unwrap())
                .collect();
            Point::new(coords[0], coords[1])
        })
        .collect();
    points.sort();
    print!(
        "{}",
        points
            .iter()
            .map(|point| format!("{}\n", point))
            .collect::<String>()
    );
}
