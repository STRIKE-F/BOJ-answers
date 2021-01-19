// 1436 영화감독 숌

use std::io;

fn main() {
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");
    let edition = input.trim().parse::<usize>().unwrap();

    let mut title_nums: Vec<u32> = Vec::with_capacity(20000 * 6);
    let num_strings: Vec<String> = (0..=19999).map(|num| format!("{:05}", num)).collect();
    num_strings.iter().for_each(|num_string| {
        (0..=num_string.len()).for_each(|pos| {
            let mut num_string = num_string.clone();
            num_string.insert_str(pos, "666");
            title_nums.push(num_string.parse().unwrap());
        })
    });

    title_nums.sort();
    title_nums.dedup();
    println!("{}", title_nums[edition - 1]);
}
