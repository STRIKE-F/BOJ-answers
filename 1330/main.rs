// 1330 두 수 비교하기

use std::io;
use std::cmp::Ordering;

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("error while readline");

    let nums: Vec<i32> = input.split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();
    let a = nums[0];
    let b = nums[1];

    println!("{}", match a.cmp(&b) {
        Ordering::Less => "<",
        Ordering::Equal => "==",
        Ordering::Greater => ">",
    });
}