// 2750 수 정렬하기

use std::io;
use std::io::prelude::*;

fn main() {
    io::stdin()
        .read_line(&mut String::new())
        .expect("readline failed: ");

    let mut nums: Vec<i16> = io::stdin()
        .lock()
        .lines()
        .map(|word| word.unwrap().parse().unwrap())
        .collect();
    nums.sort();

    let num_strings: String = nums.iter().map(|num| format!("{}\n", num)).collect();
    print!("{}", num_strings);
}
