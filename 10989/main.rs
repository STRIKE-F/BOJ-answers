// 10989 수 정렬하기 3

use std::io;
use std::io::prelude::*;

fn main() {
    io::stdin()
        .read_line(&mut String::new())
        .expect("readline failed: ");
    let mut num_counts = vec![0; 10001];

    io::stdin()
        .lock()
        .lines()
        .for_each(|word| num_counts[word.unwrap().parse::<usize>().unwrap()] += 1);

    num_counts.iter().enumerate().for_each(|(num, &count)| {
        print!(
            "{}",
            (0..count).map(|_| format!("{}\n", num)).collect::<String>()
        )
    });
}
