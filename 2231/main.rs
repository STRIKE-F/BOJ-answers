// 2231 분해합

use std::io;

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("readline failed: ");
    let max_digit_sum = input.trim().len() as u32 * 9;
    let decomp_sum = input.trim().parse::<u32>().unwrap();
    let min_possible_generator = decomp_sum - max_digit_sum;

    let min_generator = (min_possible_generator..decomp_sum).find(|generator| {
        let digit_sum: u32 = generator.to_string()
            .chars().map(|c| c.to_digit(10).unwrap())
            .sum();
        generator + digit_sum == decomp_sum
    }).unwrap_or(0);
    println!("{}", min_generator);
}