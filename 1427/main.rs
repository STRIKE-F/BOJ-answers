// 1427 소트인사이드

use std::io;

fn main() {
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");
    let mut digits: Vec<char> = input.trim().chars().collect();
    digits.sort();
    digits.reverse();
    println!("{}", digits.iter().collect::<String>());
}
