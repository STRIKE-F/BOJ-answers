// D번 - Y-수열
// 20127 Y-수열

use std::io;
use std::cmp;
use std::convert::TryInto;

fn can_be_monotonic(sequence: &Vec<u32>) -> Result<u32, ()> {
    let res_non_dec = check_non_decreasing(sequence)
        .unwrap_or(u32::MAX);
    let res_non_inc = check_non_increasing(sequence)
        .unwrap_or(u32::MAX);

    if res_non_dec == u32::MAX && res_non_inc == u32::MAX {
        Err(())
    } else {
        Ok(cmp::min(res_non_dec, res_non_inc))
    }
}

fn check_non_decreasing(sequence: &Vec<u32>) -> Result<u32, ()> {
    let mut move_at: u32 = 0;
    let mut seq_iter = sequence
        .iter()
        .enumerate()
        .peekable();

    while let Some((index, num)) = seq_iter.next() {
        if let Some((_, num_next)) = seq_iter.peek() {
            if num > num_next {
                // decrease found!
                if move_at == 0 {
                    // if it is first decreasing sequence
                    if sequence.first().unwrap() >= sequence.last().unwrap() {
                        // if the subsequence is appendable
                        // (that is, first element is greater than the last element)
                        move_at = (index + 1)
                            .try_into()
                            .unwrap();
                    } else {
                        return Err(())
                    }
                } else {
                    // sequence with more than one decreasing sequence can't be converted!
                    return Err(())
                }
            }
        }
    };
    Ok(move_at)
}

fn check_non_increasing(sequence: &Vec<u32>) -> Result<u32, ()> {
    let mut move_at: u32 = 0;
    let mut seq_iter = sequence
        .iter()
        .enumerate()
        .peekable();

    while let Some((index, num)) = seq_iter.next() {
        if let Some((_, num_next)) = seq_iter.peek() {
            if num < num_next {
                // increase found!
                if move_at == 0 {
                    // if it is first increasing sequence
                    if sequence.first().unwrap() <= sequence.last().unwrap() {
                        // if the subsequence is appendable
                        // (that is, first element is lesser than the last element)
                        move_at = (index + 1)
                            .try_into()
                            .unwrap();
                    } else {
                        return Err(())
                    }
                } else {
                    // sequence with more than one increasing sequence can't be converted!
                    return Err(())
                }
            }
        }
    };
    Ok(move_at)
}

fn main() {
    // consume away unnecessary first line
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}\n", e)
    }

    input.clear();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}\n", e)
    }
    let sequence: Vec<u32> = input
        .split_whitespace()
        .map(|word| { word
            .parse()
            .unwrap()
        })
        .collect();

    match can_be_monotonic(&sequence) {
        Ok(move_at) => println!("{}", move_at),
        Err(_) => println!("-1")
    };
}