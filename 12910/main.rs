// 12910 사탕 나눠주기

use std::io;
use std::cmp::Ordering;

type Dtype = i32;

fn count_possibilities(candies: &Vec<Dtype>) -> Dtype {
    let mut possibility_count = 0;
    let mut expected_brand = 1;
    let mut possibility_until = 1;
    let mut duplicate_count = 0;

    // if first candy's brand is not 1, we can't choose any candy
    if candies.first().unwrap() != &expected_brand {
        return 0
    }
    
    for brand in candies.iter() {
        match brand.cmp(&expected_brand) {
            Ordering::Equal => {
                duplicate_count += 1;
            },
            Ordering::Greater => {
                possibility_until *= duplicate_count;
                possibility_count += possibility_until;
                duplicate_count = 1;
                expected_brand += 1;
                
                // brands are not consecutive!
                if brand > &expected_brand {
                    break
                }
            },
            Ordering::Less => panic!("Vec is supposed to be sorted!")
        }
    }
    // count possibilities by the last brand
    // unless the above loop was 
    if candies.last().unwrap() == &expected_brand {
        possibility_until *= duplicate_count;
        possibility_count += possibility_until;
    }

    possibility_count
}

fn main() {
    let mut input = String::new();
    
    // consume away number of candies
    // as it is unnecessary thanks to String split
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}\n", e)
    }
    input.clear();

    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}\n", e)
    }
    let mut candies: Vec<Dtype> = input
        .split_whitespace()
        .map(|num_str| num_str.parse().unwrap())
        .collect();
    candies.sort();

    println!("{}", count_possibilities(&candies));    
}