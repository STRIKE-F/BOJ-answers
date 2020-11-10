// C번 - 교수님의 기말고사
// 20126 교수님의 기말고사

use std::io;
use std::io::prelude::*;

struct Exam {
    time_from: u32,
    time_to: u32,
}

impl Exam {
    fn new(time_from: u32, length: u32) -> Self {
        Self {
            time_from,
            time_to: time_from + length,
        }
    }
}

struct Timeline {
    ends_at: u32,
    exams: Vec<Exam>,
}

impl Timeline {
    fn new(ends_at: u32) -> Self {
        Self {
            ends_at,
            exams: Vec::new(),
        }
    }

    // redundant
    /*
    fn add_exam(&mut self, exam: Exam) {
        self.exams.push(exam);
    }
    */

    fn add_exams(&mut self, mut exams: Vec<Exam>) {
        self.exams.append(&mut exams);
    }

    fn find_possible_time(&mut self, exam_length: u32) -> Result<u32, ()> {
        self.exams.sort_by_key(|exam| exam.time_from);
        let mut last_exam_end_time: u32 = 0;
        // if there's enough time before or between the other exams
        for exam in self.exams.iter() {
            if (exam.time_from - last_exam_end_time) >= exam_length {
                return Ok(last_exam_end_time)
            };
            last_exam_end_time = exam.time_to;
        }
        // if there's enough time after the other exams
        if (self.ends_at - last_exam_end_time) >= exam_length {
            return Ok(last_exam_end_time)
        } else {
            return Err(())
        }
    }
}

fn main() {
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}\n", e)
    };
    let params: Vec<u32> = input
        .split_whitespace()
        .map(|word| { word
            .parse()
            .unwrap()
        })
        .collect();
    // # of other exams can be ignored
    let exam_length = params[1];
    let ends_at = params[2];

    let mut timeline = Timeline::new(ends_at);
    timeline.add_exams(io::stdin()
        .lock()
        .lines()
        .map(|line| { let exam_params: Vec<u32> = line
            .unwrap()
            .split_whitespace()
            .map(|word| { word
                .parse()
                .unwrap()
            })
            .collect();
            Exam::new(exam_params[0], exam_params[1])
        })
        .collect()
    );
    match timeline.find_possible_time(exam_length) {
        Ok(time) => println!("{}", time),
        Err(_) => println!("-1")
    };
}