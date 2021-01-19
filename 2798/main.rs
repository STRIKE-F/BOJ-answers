// 2798 블랙잭

use std::io;
use std::cmp::max;

fn max_3sum_le_limit(mut nums: Vec<u32>, limit: u32) -> u32 {
    let mut max_3sum = 0;
    nums.sort();
    nums.reverse();
    nums.iter().enumerate().for_each(|(idx1, &num1)| {
        if num1 <= limit {
            nums.iter().enumerate().skip(idx1 + 1).for_each(|(idx2, &num2)| {
                if num1 + num2 <= limit {
                    nums.iter().skip(idx2 + 1).for_each(|&num3| {
                        if num1 + num2 + num3 <= limit {
                            max_3sum = max(max_3sum, num1 + num2 + num3)
                        }
                    })
                }
            })
        }
    });
    max_3sum
}

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("error while readline: ");
    let sum_limit: u32 = input.trim()
        .split_whitespace().nth(1).unwrap()
        .parse().unwrap();
    input.clear();

    io::stdin().read_line(&mut input).expect("error while readline: ");
    let cards: Vec<u32> = input.trim()
        .split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();
    println!("{}", max_3sum_le_limit(cards, sum_limit));
}