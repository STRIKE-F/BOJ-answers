// F번 - 뒤집힌 계산기
// 20129 뒤집힌 계산기

use std::io;
use std::iter::Peekable;
use std::slice::Iter;

type PType = u8;
type OType = i64;

enum OperatorType {
    Add,
    Sub,
    Mul,
    Div,
}

impl OperatorType {
    fn as_char(&self) -> char {
        match self {
            Self::Add => '+',
            Self::Sub => '-',
            Self::Mul => '*',
            Self::Div => '/',
        }
    }

    fn operate(&self, lhs: OType, rhs: OType) -> OType {
        match self {
            Self::Add => lhs + rhs,
            Self::Sub => lhs - rhs,
            Self::Mul => lhs * rhs,
            Self::Div => lhs / rhs,            
        }
    }
}

struct PriorityOperator {
    priority: PType,
    op: OperatorType,
}

#[allow(non_snake_case)]
impl PriorityOperator {
    fn Add(priority: PType) -> Self {
        Self {
            priority,
            op: OperatorType::Add,
        }
    }

    fn Sub(priority: PType) -> Self {
        Self {
            priority,
            op: OperatorType::Sub,
        }
    }

    fn Mul(priority: PType) -> Self {
        Self {
            priority,
            op: OperatorType::Mul,
        }
    }

    fn Div(priority: PType) -> Self {
        Self {
            priority,
            op: OperatorType::Div,
        }
    }

    fn op_char(&self) -> char {
        self.op.as_char()
    }

    fn operate(&self, lhs: OType, rhs: OType) -> OType {
        self.op.operate(lhs, rhs)
    }
}

struct PriorityCalculator {
    priority_operators: Vec<PriorityOperator>,
}

impl PriorityCalculator {
    fn new(mut priority_operators: Vec<PriorityOperator>) -> Self {
        priority_operators.sort_by_key(|po| po.priority);
        Self {
            priority_operators,
        }
    }

    fn reduce(&self, subexpr: &str, current_op: &Peekable<Iter<PriorityOperator>>) -> OType {
        let mut next_op = current_op.clone();
        let po = next_op.next().unwrap();

        if let None = next_op.peek() {
            let mut subexpr_iter = subexpr
                .rsplit(po.op_char())
                .map(|operand| { operand
                    .parse::<OType>()
                    .unwrap()
                });
            let init = subexpr_iter
                .next()
                .unwrap();
            subexpr_iter.fold(init, |acc, x| po.operate(acc, x))
        } else {
            let mut subexpr_iter = subexpr
                .rsplit(po.op_char())
                .map(|subexpr| self.reduce(subexpr, &next_op));
            let init = subexpr_iter
                .next()
                .unwrap();
            subexpr_iter.fold(init, |acc, x| po.operate(acc, x))
        }
    }

    fn calculate(&self, input: &str) -> OType {
        self.reduce(input, &self.priority_operators.iter().peekable())
    }
}

fn main() {
    let mut input = String::new();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}", e)
    };
    let priorities: Vec<PType> = input
        .split_whitespace()
        .map(|word| { word
            .parse()
            .unwrap()
        })
        .collect();
    let calculator = PriorityCalculator::new(
        vec![PriorityOperator::Add(priorities[0]),
            PriorityOperator::Sub(priorities[1]),
            PriorityOperator::Mul(priorities[2]),
            PriorityOperator::Div(priorities[3])
        ]
    );

    input.clear();
    if let Err(e) = io::stdin().read_line(&mut input) {
        panic!("error: {}", e)
    };
    println!("{}", calculator.calculate(input.trim()));
}