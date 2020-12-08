// 1110 더하기 사이클

use std::io;

fn calculate_cycle_length_of(num_in_str: &str) -> usize {
    let mut cycle_length = 0;
    let first_num = num_in_str
        .parse::<u8>()
        .unwrap();
    let mut new_num = first_num;

    loop {
        let digit_sum = new_num / 10 + new_num % 10;
        new_num = new_num % 10 * 10 + digit_sum % 10;
        cycle_length += 1;

        if first_num == new_num {
            break;
        }
    };

    cycle_length
}

fn main() {
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}", e)
    };
    
    println!("{}", calculate_cycle_length_of(&input.trim()));
}