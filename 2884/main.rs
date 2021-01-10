// 2884 알람 시계

use std::io;

fn main() {
    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("error while readline");

    let time: Vec<u32> = input.split_whitespace()
        .map(|word| word.parse().unwrap())    
        .collect();
    let mut hour = time[0];
    let mut minute = time[1];

    minute = minute.checked_sub(45)
        .unwrap_or_else(|| {
            hour = hour.checked_sub(1).unwrap_or(23);
            minute + 15
        });
    println!("{} {}", hour, minute)
}