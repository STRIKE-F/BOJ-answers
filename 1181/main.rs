// 1181 단어 정렬

use std::io;
use std::io::prelude::*;

fn main() {
    // # of words is redundant
    io::stdin()
        .read_line(&mut String::new())
        .expect("readline failed: ");

    let mut words: Vec<String> = io::stdin()
        .lock()
        .lines()
        .map(|line| line.unwrap())
        .collect();
    words.sort_unstable();
    words.dedup();
    words.sort_by_key(|word| word.len());
    print!(
        "{}",
        words
            .iter()
            .map(|word| format!("{}\n", word))
            .collect::<String>()
    );
}
