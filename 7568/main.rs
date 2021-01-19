// 7568 덩치

use std::io;
use std::io::prelude::*;
use std::convert::From;
use std::cmp::{Ord, Ordering};

#[derive(PartialEq, Eq)]
struct Mass {
    weight: u8,
    height: u8,
}

// should be TryFrom for real-life program,
// but the input is assured to be Vec of len=2
// so we impl From for the sake of simplicity
impl From<Vec<u8>> for Mass {
    fn from(wt_ht: Vec<u8>) -> Self {
        Self {
            weight: wt_ht[0],
            height: wt_ht[1],
        }
    }
}

impl PartialOrd for Mass {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        let wt_order = self.weight.cmp(&other.weight);
        let ht_order = self.height.cmp(&other.height);
        if wt_order == ht_order {
            Some(wt_order)
        } else {
            Some(Ordering::Equal)
        }
    }
}

impl Ord for Mass {
    fn cmp(&self, other: &Self) -> Ordering {
        self.partial_cmp(other).unwrap()
    }
}

fn main() {
    io::stdin().read_line(&mut String::new()).expect("readline failed: ");

    let masses: Vec<Mass> = io::stdin().lock().lines()
        .map(|line| {
            let wt_ht = line.unwrap().split_whitespace()
                .map(|word| word.parse().unwrap())
                .collect::<Vec<u8>>();
            Mass::from(wt_ht)
        })
        .collect();
    
    let ranks: Vec<usize> = masses.iter()
        .map(|mass| masses.iter().filter(|other| other > &mass).count() + 1)
        .collect();
    let rank_string: String = ranks.iter().map(|rank| format!("{} ", rank)).collect();
    println!("{}", rank_string);
}