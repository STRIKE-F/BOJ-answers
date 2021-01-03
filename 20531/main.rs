// 20531 인간관계

use std::io;
use std::io::prelude::*;
use std::collections::BTreeSet;

type IDType = u16;
type IntType = u64;
const MOD: IntType = 1_000_000_007;

fn stirling(n: usize, k: usize, table: &mut Vec<Vec<Option<IntType>>>) -> IntType {
    table[n][k].unwrap_or_else(|| {
        let possibile_split_num = (stirling(n-1, k-1, table) + stirling(n-1, k, table) * (k as IntType)) % MOD;
        table[n][k] = Some(possibile_split_num);
        possibile_split_num
    })
}

fn create_stirling_table(set_num: usize) -> Vec<Vec<Option<IntType>>> {
    // create half-square table for size and performance optimization
    let mut table: Vec<Vec<Option<IntType>>> = (0..set_num+1)
        .map(|n| {
            match n {
                0 => vec![Some(1), Some(0)],
                1 => vec![Some(0), Some(1), Some(0)],
                _ => {
                    let mut row = vec![Some(0), Some(1)];
                    row.reserve(n);
                    (0..n.checked_sub(2).unwrap_or(0)).for_each(|_| row.push(None));
                    row.push(Some(1));
                    row.push(Some(0));
                    row
                }
            }
        })
        .collect();
    
    // run stirling number algorithm
    (0..set_num).for_each(|k| {
        stirling(set_num, k, &mut table);
    });
    table
}

fn create_bell_table(stirling_table: &Vec<Vec<Option<IntType>>>) -> Vec<IntType> {
    stirling_table.iter()
        .map(|row| row.iter().fold(0, |acc, val| acc + val.unwrap_or(0)) % MOD)
        .collect()
}

struct Connection {
    s1: IDType,
    s2: IDType,
}

impl Connection {
    fn from(line: &str) -> Result<Self, ()> {
        let parsed_line: Vec<IDType> = line
            .split_whitespace()
            .map(|word| word.parse().unwrap())
            .collect();
        let s1 = parsed_line[0];
        let s2 = parsed_line[1];

        // discard connection to itself
        if s1 == s2 {
            Err(())
        } else {
            Ok(Self {
                s1,
                s2,
            })
        }        
    }
}

struct Network {
    students: BTreeSet<IDType>,
    connections: Vec<BTreeSet<IDType>>,
}

impl Network {
    fn new(student_num: usize) -> Self {
        Self {
            students: (1..student_num+1).map(|s| s as u16).collect(),
            connections: Vec::new(),
        }
    }

    fn add(&mut self, conn: Connection) {
        let s1_friends = if self.students.contains(&conn.s1) {
            self.students.remove(&conn.s1);
            None
        } else {
            let set_idx = self.connections.iter()
                .position(|connection| connection.contains(&conn.s1))
                .unwrap();
            Some(self.connections.remove(set_idx))
        };

        let s2_friends = if self.students.contains(&conn.s2) {
            self.students.remove(&conn.s2);
            None
        } else {
            if let Some(set_idx) = self.connections.iter()
                .position(|connection| connection.contains(&conn.s2)) {
                Some(self.connections.remove(set_idx))
            } else {
                // this pair is already connected together
                self.connections.push(s1_friends.unwrap());
                return
            }
        };

        let mut connection;
        if s1_friends.is_some() && s2_friends.is_some() {
            connection = s1_friends.unwrap();
            connection.append(&mut s2_friends.unwrap());
        } else if s1_friends.is_some() {
            connection = s1_friends.unwrap();
            connection.insert(conn.s2);
        } else if s2_friends.is_some() {
            connection = s2_friends.unwrap();
            connection.insert(conn.s1);
        } else {
            connection = BTreeSet::new();
            connection.insert(conn.s1);
            connection.insert(conn.s2);
        }
        self.connections.push(connection);
    }

    fn count_possible_connections(&self, bell_table: &Vec<IntType>) -> IntType {
        let set_num = self.connections.len() + self.students.len();
        bell_table[set_num]
    }
}

fn main() {
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}\n", e)
    }
    let student_num = input
        .split_whitespace()
        .nth(0)
        .unwrap()
        .parse::<usize>()
        .unwrap();
    let mut network = Network::new(student_num);
    let stirling_table = create_stirling_table(student_num);
    let bell_table = create_bell_table(&stirling_table);

    let ans: String = io::stdin().lock().lines()
        .map(|line| {
            if let Ok(conn) = Connection::from(&line.unwrap()) {
                network.add(conn);
            }
            format!("{}\n", network.count_possible_connections(&bell_table))
        })
        .collect();
    print!("{}", ans);
}