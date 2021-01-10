// 10998 A×B

use std::io;

fn main() {
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("{}", e)
    }

    let product: i32 = input
        .split_whitespace()
        .map(|word| word.parse::<i32>().unwrap())
        .product();

    println!("{}", product);
}