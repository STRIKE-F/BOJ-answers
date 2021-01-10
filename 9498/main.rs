// 9498 시험 성적

use std::io;

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("error while readline");

    let score = input.trim().parse::<i32>().unwrap();
    println!("{}", match score {
        90..=100 => "A",
        80..=89 => "B",
        70..=79 => "C",
        60..=69 => "D",
        _ => "F",
    });
}