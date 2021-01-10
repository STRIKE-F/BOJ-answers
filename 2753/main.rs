// 2753 윤년

use std::io;

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("error while readline");

    let year = input.trim().parse::<i32>().unwrap();
    println!("{}", if year % 400 == 0 {
        1
    } else if year % 100 == 0 {
        0
    } else if year % 4 == 0 {
        1
    } else {
        0
    });
}