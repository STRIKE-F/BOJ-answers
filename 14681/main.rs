// 14681 사분면 고르기

use std::io;
use std::io::prelude::*;
use std::cmp::Ordering;

fn main() {
    let coords: Vec<i32> = io::stdin().lock()
        .lines()
        .map(|word| word.unwrap().parse().unwrap())
        .collect();
    let (x, y) = (coords[0], coords[1]);

    println!("{}", match (x.cmp(&0), y.cmp(&0)) {
        (Ordering::Greater, Ordering::Greater) => 1,
        (Ordering::Less, Ordering::Greater) => 2,
        (Ordering::Less, Ordering::Less) => 3,
        (Ordering::Greater, Ordering::Less) => 4,
        _ => panic!("Invalid coordinates!"),
    });
}