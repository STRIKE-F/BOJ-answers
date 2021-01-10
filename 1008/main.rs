// 1008 A/B

use std::io;

fn main() {
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("{}", e)
    }

    let nums: Vec<f64> = input
        .split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();

    println!("{}", nums[0] / nums[1])
}