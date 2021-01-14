// 2879 코딩은 예쁘게

use std::io;
use std::cmp::Ordering;

type IndType = i8;
type OffType = u8;

enum OffsetKind {
    Positive,
    Zero,
    Negative,
}

impl PartialEq for OffsetKind {
    fn eq(&self, other: &Self) -> bool {
        let lhs = match self {
            Self::Positive => 1,
            Self::Zero => 0,
            Self::Negative => -1,
        };
        let rhs = match other {
            Self::Positive => 1,
            Self::Zero => 0,
            Self::Negative => -1,
        };
        lhs == rhs
    }
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
    let mut offset_kind = OffsetKind::Zero;
    for offset in offsets.iter() {
        match offset.cmp(&0) {
            // negative offset
            Ordering::Less => {
                let offset = offset.abs() as OffType;
                match offset_kind {
                    OffsetKind::Positive => {
                        edits += align(&mut monotonic_offsets, 0);
                        offset_kind = OffsetKind::Negative;
                        monotonic_offsets.clear();
                        monotonic_offsets.push(offset);
                    },
                    OffsetKind::Negative => {
                        match monotonic_offsets.last().unwrap().cmp(&offset) {
                            Ordering::Less => monotonic_offsets.push(offset),
                            Ordering::Equal => (),
                            Ordering::Greater => {
                                edits += align(&mut monotonic_offsets, offset);
                                monotonic_offsets.push(offset)
                            }
                        }
                    },
                    OffsetKind::Zero => {
                        offset_kind = OffsetKind::Negative;
                        monotonic_offsets.push(offset);
                    }
                }
            },
            // positive offset
            Ordering::Greater => {
                let offset = *offset as OffType;
                match offset_kind {
                    OffsetKind::Positive => {
                        match monotonic_offsets.last().unwrap().cmp(&offset) {
                            Ordering::Less => monotonic_offsets.push(offset),
                            Ordering::Equal => (),
                            Ordering::Greater => {
                                edits += align(&mut monotonic_offsets, offset);
                                monotonic_offsets.push(offset)
                            }
                        }
                    },
                    OffsetKind::Negative => {
                        edits += align(&mut monotonic_offsets, 0);
                        offset_kind = OffsetKind::Positive;
                        monotonic_offsets.clear();
                        monotonic_offsets.push(offset);
                    },
                    OffsetKind::Zero => {
                        offset_kind = OffsetKind::Positive;
                        monotonic_offsets.push(offset);
                    }
                }
            },
            // zero offset
            Ordering::Equal => {
                match offset_kind {
                    OffsetKind::Positive | OffsetKind::Negative => {
                        edits += align(&mut monotonic_offsets, 0);
                        offset_kind = OffsetKind::Zero;
                        monotonic_offsets.clear();
                    },
                    OffsetKind::Zero => ()
                }
            }
        }
    }

    // end-of-line(?) cleanup
    match offset_kind {
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