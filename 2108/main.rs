// 2108 통계학

use std::io;
use std::io::prelude::*;

fn main() {
    // # of nums is redundant
    io::stdin()
        .read_line(&mut String::new())
        .expect("readline failed: ");

    let mut nums: Vec<i32> = io::stdin()
        .lock()
        .lines()
        .map(|word| word.unwrap().parse().unwrap())
        .collect();
    nums.sort_unstable();

    let average_fl = nums.iter().sum::<i32>() as f32 / nums.len() as f32;
    let average = average_fl.round() as i32;
    println!("{}", average);

    let median = nums[nums.len() / 2];
    println!("{}", median);

    let mut nums_dedup = nums.clone();
    nums_dedup.dedup();
    let freqs: Vec<(i32, usize)> = nums_dedup
        .iter()
        .map(|num_to_find| {
            (
                *num_to_find,
                nums.iter().filter(|&num| num == num_to_find).count(),
            )
        })
        .collect();
    let (_, max_dup) = freqs.iter().max_by_key(|(_, dup)| dup).unwrap();
    let max_dups: Vec<i32> = freqs
        .iter()
        .filter(|(_, dup)| dup == max_dup)
        .map(|(num, _)| *num)
        .collect();
    let most_freq = if max_dups.len() > 1 {
        max_dups[1]
    } else {
        max_dups[0]
    };
    println!("{}", most_freq);

    let range = nums.last().unwrap() - nums.first().unwrap();
    println!("{}", range);
}
