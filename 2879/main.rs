// 2879 코딩은 예쁘게

use std::io;
use std::cmp::Ordering;

type IndType = i8;
type OffType = u8;

#[derive(PartialEq)]
enum OffsetKind {
    Positive,
    Zero,
    Negative,
}

fn align(offsets: &mut Vec<OffType>, until: OffType) -> u32 {
    let mut edits = 0;

    let mut offsets_to_align = offsets.split_off(offsets.iter()
        .position(|&offset| offset > until)
        .unwrap());
    while !offsets_to_align.is_empty() {
        let current_offset = offsets_to_align.pop().unwrap();
        // subtraction never fails, as offsets_to_align is decreasing seq.
        if let Some(prev_offset) = offsets_to_align.last() {
            edits += (current_offset - prev_offset) as u32;
        } else {
            edits += (current_offset - until) as u32;
        }
    }

    edits
}

fn minimum_edits(offsets: Vec<IndType>) -> u32 {
    let mut edits = 0;
    let mut monotonic_offsets: Vec<OffType> = Vec::new();
    let mut prev_offset = OffsetKind::Zero;
    for offset in offsets.iter() {
        let current_offset = match offset.cmp(&0) {
            Ordering::Less => OffsetKind::Negative,
            Ordering::Equal => OffsetKind::Zero,
            Ordering::Greater => OffsetKind::Positive,
        };

        if current_offset == OffsetKind::Zero { 
            if prev_offset != OffsetKind::Zero {
                prev_offset = OffsetKind::Zero;
                edits += align(&mut monotonic_offsets, 0);
                monotonic_offsets.clear();
            }
        } else {
            let offset = offset.abs() as OffType;
            if prev_offset == OffsetKind::Zero {
                prev_offset = current_offset;
                monotonic_offsets.push(offset);
            } else {
                if prev_offset == current_offset {
                    match monotonic_offsets.last().unwrap().cmp(&offset) {
                        Ordering::Less => monotonic_offsets.push(offset),
                        Ordering::Equal => (),
                        Ordering::Greater => {
                            edits += align(&mut monotonic_offsets, offset);
                            monotonic_offsets.push(offset)
                        }
                    }
                } else {
                    prev_offset = current_offset;
                    edits += align(&mut monotonic_offsets, 0);
                    monotonic_offsets.clear();
                    monotonic_offsets.push(offset);
                }
            }
        }
    }
    // end-of-line(?) cleanup
    match prev_offset {
        OffsetKind::Positive | OffsetKind::Negative => {
            edits += align(&mut monotonic_offsets, 0);
        },
        OffsetKind::Zero => ()
    }
    edits
}

fn main() {
    // ignore # of lines as it is redundant
    io::stdin().read_line(&mut String::new()).expect("error while readline:");

    let mut current_indents = String::new();
    io::stdin().read_line(&mut current_indents).expect("error while readline:");
    let current_indents: Vec<IndType> = current_indents.trim()
        .split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();

    let mut proper_indents = String::new();
    io::stdin().read_line(&mut proper_indents).expect("error while readline:");
    let proper_indents: Vec<IndType> = proper_indents.trim()
        .split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();

    let indent_offsets: Vec<IndType> = current_indents.iter()
        .zip(proper_indents.iter())
        .map(|(&c, &p)| p-c)
        .collect();

    println!("{}", minimum_edits(indent_offsets));
}