// B번 - 쿠키의 신체 측정
// 20125 쿠키의 신체 측정

use std::io;
use std::io::prelude::*;
use std::convert::TryInto;
use std::iter::Iterator;

struct Grid {
    squares: Vec<Vec<bool>>,
}

impl Grid {
    fn from(lines: io::Lines<io::StdinLock>) -> Self {
        Self {
            squares: lines
                .map(|line| { line
                    .unwrap()
                    .chars()
                    .map(|c| {
                        match c {
                            '*' => true,
                            '_' => false,
                            _ => panic!("invalid input!")
                        }
                    })
                    .collect()
                })
                .collect()
        }
    }
}

struct CookieProperties {
    heart_pos: [u32; 2],
    arm_length: [u32; 2],
    waist_length: u32,
    leg_length: [u32; 2],
}

impl CookieProperties {
    pub fn from(grid: &Grid) -> Self {
        let mut ret = Self {
            heart_pos: [0, 0],
            arm_length: [0, 0],
            waist_length: 0,
            leg_length: [0, 0],
        };

        let mut line_iter = grid.squares.iter().enumerate();
        ret.find_head(&mut line_iter);
        ret.find_arms(&mut line_iter);
        ret.find_waist(&mut line_iter);
        ret.find_legs(&mut line_iter);
        ret
    }

    fn find_head<'a, E>(&mut self, line_iter: &mut E)
    where
        E: Iterator<Item = (usize, &'a Vec<bool>)> {
        loop {
            let (index, line) = line_iter.next().unwrap();
            if let Some(i) = line.iter().position(|b| *b) {
                self.heart_pos[0] = i.try_into().unwrap();
                self.heart_pos[1] = (index + 1).try_into().unwrap();
                break;
            }
        }
    }

    fn find_arms<'a, E>(&mut self, line_iter: &mut E)
    where
        E: Iterator<Item = (usize, &'a Vec<bool>)> {
        let (_index, line) = line_iter.next().unwrap();
        self.arm_length[0] = self.heart_pos[0]
            .checked_sub(
                line
                .iter()
                .position(|b| *b)
                .unwrap()
                .try_into()
                .unwrap()
            )
            .unwrap();
        self.arm_length[1] = TryInto::<u32>::try_into(
            line
            .iter()
            .rposition(|b| *b)
            .unwrap()) 
            .unwrap()
            .checked_sub(self.heart_pos[0])
            .unwrap();
    }

    fn find_waist<'a, E>(&mut self, line_iter: &mut E)
    where
        E: Iterator<Item = (usize, &'a Vec<bool>)> {
        loop {
            let (index, line) = line_iter.next().unwrap();
            let left = line
                .iter()
                .position(|b| *b)
                .unwrap();
            let right = line
                .iter()
                .rposition(|b| *b)
                .unwrap();
            
            if left != right {
                self.waist_length = TryInto::<u32>::try_into(index - 1) 
                    .unwrap()
                    .checked_sub(self.heart_pos[1])
                    .unwrap();
                break;
            }
        }
    }

    fn find_legs<'a, E>(&mut self, line_iter: &mut E)
    where
        E: Iterator<Item = (usize, &'a Vec<bool>)> {
        self.leg_length = [1, 1];
        loop {
            let (_index, line) = if let Some(pair) = line_iter.next() {
                pair
            } else {
                // end of grid
                break
            };

            let left: usize = (self.heart_pos[0] - 1).try_into().unwrap();
            let right: usize = (self.heart_pos[0] + 1).try_into().unwrap();

            if line[left] {
                self.leg_length[0] += 1;
            }
            if line[right] {
                self.leg_length[1] += 1;
            }
            if !line[left] && !line[right] {
                break
            }
        }
    }
}

fn main() {
    // consume away unnecessary first line
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}\n", e)
    }

    let grid = Grid::from(io::stdin().lock().lines());
    let properties = CookieProperties::from(&grid);
    println!("{} {}", 
        properties.heart_pos[1] + 1,
        properties.heart_pos[0] + 1
    );
    println!("{} {} {} {} {}", 
        properties.arm_length[0],
        properties.arm_length[1],
        properties.waist_length,
        properties.leg_length[0],
        properties.leg_length[1]
    );

}