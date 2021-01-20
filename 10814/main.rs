// 10814 나이순 정렬

use std::fmt::{Display, Error, Formatter};
use std::io;
use std::io::prelude::*;

#[derive(PartialOrd, Ord, PartialEq, Eq)]
struct User {
    age: u8,
    id: usize,
    name: String,
}

impl User {
    fn new(id: usize, age: u8, name: String) -> Self {
        Self { age, id, name }
    }
}

impl Display for User {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), Error> {
        write!(f, "{} {}", self.age, self.name)
    }
}

fn main() {
    // # of users is redundant
    io::stdin()
        .read_line(&mut String::new())
        .expect("readline failed: ");

    let mut users: Vec<User> = io::stdin()
        .lock()
        .lines()
        .enumerate()
        .map(|(id, line)| {
            let line = line.unwrap();
            let words: Vec<&str> = line.split_whitespace().collect();
            let age = words[0].parse().unwrap();
            let name = words[1].to_string();
            User::new(id, age, name)
        })
        .collect();

    users.sort();
    print!(
        "{}",
        users
            .iter()
            .map(|user| format!("{}\n", user))
            .collect::<String>()
    );
}
