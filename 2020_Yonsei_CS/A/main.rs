// A번 - 모르고리즘 회장님 추천 받습니다
// 20124 모르고리즘 회장님 추천 받습니다

use std::io;
use std::io::prelude::*;

struct Candidate {
    name: String,
    score: i32,
}

impl Candidate {
    fn from(name: String, score: i32) -> Self {
        Self {
            name,
            score,
        }
    }
}

fn candidate_list_from(lines: io::Lines<io::StdinLock>) -> Vec<Candidate> {
    lines
    .map(|line| line.unwrap())
    .map(|line| {
        let pair: Vec<&str> = line
            .split_whitespace()
            .collect();
        let name = pair[0]
            .to_owned();
        let score = pair[1]
            .parse()
            .unwrap();
        Candidate::from(name, score)
    })
    .collect()
}

fn choose_president_from(mut candidates: Vec<Candidate>) -> String {
    candidates.sort_by(|c1, c2| c1.name.cmp(&c2.name));
    candidates.sort_by_key(|c| -c.score);
    candidates.first().unwrap().name.clone()
}

fn main() {
    // consume away unnecessary first line
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}\n", e)
    }

    let candidates = candidate_list_from(io::stdin().lock().lines());
    println!("{}", choose_president_from(candidates));
}