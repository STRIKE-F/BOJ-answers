// 4312 3의 제곱

use std::io;
use std::convert::TryFrom;

type IType = i64;
type OType = i128;

fn nth_partial_set(input: IType) -> Vec<OType> {
    let mut partial_set = Vec::new();
    let binary_representation = format!("{:b}", input - 1);
    for (power, bit) in binary_representation.chars().rev().enumerate() {
        if bit == '1' {
            let element = OType::pow(3, u32::try_from(power).unwrap());
            partial_set.push(element);
        }
    }

    partial_set
}

fn print_sets(partial_sets: Vec<Vec<OType>>) {
    for set in partial_sets.iter() {
        if set.is_empty() {
            println!("{{ }}");
            continue;
        }

        let mut line = String::from("{ ");
        let mut iter = set.iter().peekable();

        loop {
            line.push_str(&format!("{}", iter.next().unwrap()));
            if iter.peek().is_some() {
                line.push_str(", ");
            } else {
                line.push_str(" }");
                break;
            }
        }

        println!("{}", line);
    }
}

fn main() {
    let mut partial_sets = Vec::new();

    loop {
        let mut line = String::new();
        match io::stdin().read_line(&mut line) {
            Ok(_) => {
                // println!("{}", input)
            },
            Err(error) => {
                panic!("error: {}", error)
            }
        };
        
        // Input is guaranteed to be clean, so no need to match Result
        let input = IType::from_str_radix(line.trim_end(), 10).unwrap();
        if input == 0 {
            break;
        }

        partial_sets.push(nth_partial_set(input));
    }

    print_sets(partial_sets);
}