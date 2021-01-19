// 1018 체스판 다시 칠하기

use std::cmp::min;
use std::convert::From;
use std::io;
use std::io::prelude::*;

#[derive(Debug)]
enum Tile {
    Black,
    White,
}

impl From<char> for Tile {
    fn from(c: char) -> Self {
        match c {
            'B' => Self::Black,
            'W' => Self::White,
            _ => panic!("invalid input!"),
        }
    }
}

fn count_diff(board: &[&[Tile]]) -> u32 {
    let mut diff_black_first = 0;
    let mut diff_white_first = 0;
    board.iter().enumerate().for_each(|(y, row)| {
        row.iter().enumerate().for_each(|(x, color)| {
            if (x + y) % 2 == 0 {
                match color {
                    Tile::Black => diff_white_first += 1,
                    Tile::White => diff_black_first += 1,
                }
            } else {
                match color {
                    Tile::Black => diff_black_first += 1,
                    Tile::White => diff_white_first += 1,
                }
            }
        })
    });
    min(diff_black_first, diff_white_first)
}

fn main() {
    // board size is redundant and is obtainable from input
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");

    let params: Vec<usize> = input
        .trim()
        .split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();
    // # of offsets this board can have
    let y_offsets = params[0] - 8 + 1;
    let x_offsets = params[1] - 8 + 1;

    let board: Vec<Vec<Tile>> = io::stdin()
        .lock()
        .lines()
        .map(|line| line.unwrap().chars().map(|c| Tile::from(c)).collect())
        .collect();

    // the max diff from 8x8 board is 64
    let mut min_diff_count = 64;
    (0..y_offsets).for_each(|y_off| {
        (0..x_offsets).for_each(|x_off| {
            let piece: Vec<&[Tile]> = board[y_off..y_off + 8]
                .iter()
                .map(|row| &row[x_off..x_off + 8])
                .collect();
            min_diff_count = min(min_diff_count, count_diff(&piece));
        })
    });

    println!("{}", min_diff_count);
}
