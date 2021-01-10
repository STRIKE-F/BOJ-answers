// 2588 곱셈

use std::io;

fn main() {
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("{}", e)
    }
    let a: u32 = input.trim().parse().unwrap();

    input.clear();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("{}", e)
    }

    input.trim().chars().rev()
        .for_each(|c| {
            let digit = c.to_digit(10).unwrap();
            println!("{}", a * digit);
        });

    let b: u32 = input.trim().parse().unwrap();
    println!("{}", a * b);
}