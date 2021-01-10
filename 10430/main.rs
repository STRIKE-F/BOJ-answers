// 10430 나머지

use std::io;

fn main() {
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("{}", e)
    }

    let nums: Vec<i32> = input
        .split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();
    let a = nums[0];
    let b = nums[1];
    let c = nums[2];

    println!("{}", (a + b) % c);
    println!("{}", ((a % c) + (b % c)) % c);
    println!("{}", (a * b) % c);
    println!("{}", ((a % c) * (b % c)) % c);
}