// 14889 스타트와 링크

use std::io;
use std::io::prelude::*;

fn main() {
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");
    let num_of_players: usize = input.trim().parse().unwrap();

    let synergy_points: Vec<Vec<u32>> = io::stdin()
        .lock()
        .lines()
        .map(|line| {
            line.unwrap()
                .split_whitespace()
                .map(|word| word.parse().unwrap())
                .collect()
        })
        .collect();

    let reduced_synergy_points: Vec<Vec<u32>> = (0..num_of_players)
        .map(|row_idx| {
            (0..row_idx)
                .map(|column_idx| {
                    synergy_points[row_idx][column_idx] + synergy_points[column_idx][row_idx]
                })
                .chain(std::iter::once(0))
                .collect()
        })
        .collect();

    let num_of_1s = (num_of_players / 2) as u32;
    // minimum of all the integers that have 'team size' of 1s.
    let range_min = 2u32.pow(num_of_1s) - 1;
    // num_of_players <= 20, so u32 will never overflow
    let range_max = range_min << num_of_1s;
    // iterate through min ~ max,
    // and calculate synergy only with the numbers that have 'team size' of 1s
    let synergy_gap = (range_min..=range_max)
        .filter(|num| num.count_ones() == num_of_1s)
        // filter out the 'flipped team' cases
        .filter(|num| num % 2 == 0)
        .map(|partition_binary_repr| {
            let (team_0, team_1): (Vec<usize>, Vec<usize>) = (0..num_of_players)
                .partition(|player_idx| (partition_binary_repr >> player_idx) & 1 == 0);
            let team_0_synergy = team_0
                .iter()
                .enumerate()
                .map(|(p1_idx, &p1)| {
                    team_0
                        .iter()
                        .take(p1_idx)
                        .map(|&p2| reduced_synergy_points[p1][p2])
                        .sum::<u32>()
                })
                .sum::<u32>();
            let team_1_synergy = team_1
                .iter()
                .enumerate()
                .map(|(p1_idx, &p1)| {
                    team_1
                        .iter()
                        .take(p1_idx)
                        .map(|&p2| reduced_synergy_points[p1][p2])
                        .sum::<u32>()
                })
                .sum::<u32>();

            let synergy_gap = team_0_synergy
                .checked_sub(team_1_synergy)
                .unwrap_or_else(|| team_1_synergy - team_0_synergy);
            synergy_gap
        })
        .min()
        .unwrap();
    println!("{}", synergy_gap);
}
