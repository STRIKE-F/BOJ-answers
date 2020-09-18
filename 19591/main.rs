// 19591 독특한 계산기

use std::io;
use std::str::Chars;
use std::iter::Peekable;
use std::collections::VecDeque;

enum Term {
    Operand(i64),
    Operator(OperatorKind),
}

impl Term {
    fn new_operator(symbol: char) -> Self {
        match symbol {
            '+' => Self::Operator(OperatorKind::Add),
            '-' => Self::Operator(OperatorKind::Sub),
            '*' => Self::Operator(OperatorKind::Mul),
            '/' => Self::Operator(OperatorKind::Div),
            _ => panic!("Invalid operator!")
        }
    }
}

impl Clone for Term {
    fn clone(&self) -> Self {
        match self {
            Self::Operand(number) => {
                Self::Operand(number.clone())
            },
            Self::Operator(opkind) => {
                Self::Operator(opkind.clone())
            }
        }
    }
}

#[derive(Clone)]
enum OperatorKind {
    Add,
    Sub,
    Mul,
    Div,
}

impl OperatorKind {
    fn precedence(&self) -> usize {
        match self {
            Self::Add | Self::Sub => 0,
            Self::Mul | Self::Div => 1,
        }
    }
}

struct SingleExpr {
    lhs: i64,
    operator: OperatorKind,
    rhs: i64,
}

impl SingleExpr {
    fn from_front(expression: &VecDeque<Term>) -> Self {
        let mut iter = expression.iter();

        let lhs = if let Some(term) = iter.next() {
            match term {
                Term::Operand(number) => number.clone(),
                Term::Operator(_) => panic!("Expected operand!")
            }
        } else {
            panic!("Invalid expression!")
        };

        let operator = if let Some(term) = iter.next() {
            match term {
                Term::Operator(opkind) => opkind.clone(),
                Term::Operand(_) => panic!("Expected operator!")
            }
        } else {
            panic!("Invalid expression!")
        };

        let rhs = if let Some(term) = iter.next() {
            match term {
                Term::Operand(number) => number.clone(),
                Term::Operator(_) => panic!("Expected operand!")
            }
        } else {
            panic!("Invalid expression!")
        };

        Self {
            lhs,
            operator,
            rhs,
        }
    }

    fn from_back(expression: &VecDeque<Term>) -> Self {
        let mut iter = expression.iter().rev();

        let rhs = if let Some(term) = iter.next() {
            match term {
                Term::Operand(number) => number.clone(),
                Term::Operator(_) => panic!("Expected operand!")
            }
        } else {
            panic!("Invalid expression!")
        };

        let operator = if let Some(term) = iter.next() {
            match term {
                Term::Operator(opkind) => opkind.clone(),
                Term::Operand(_) => panic!("Expected operator!")
            }
        } else {
            panic!("Invalid expression!")
        };

        let lhs = if let Some(term) = iter.next() {
            match term {
                Term::Operand(number) => number.clone(),
                Term::Operator(_) => panic!("Expected operand!")
            }
        } else {
            panic!("Invalid expression!")
        };

        Self {
            lhs,
            operator,
            rhs,
        }
    }

    fn calculate(&self) -> i64 {
        match self.operator {
            OperatorKind::Add => self.lhs + self.rhs,
            OperatorKind::Sub => self.lhs - self.rhs,
            OperatorKind::Mul => self.lhs * self.rhs,
            OperatorKind::Div => self.lhs / self.rhs,
        }
    }
}

enum ExprSide {
    Front,
    Back,
}

fn parse_line(line: &String) -> VecDeque<Term> {
    let mut expression = VecDeque::new();
    let mut iter = line.chars().peekable();

    while iter.peek().is_some() {
        match iter.peek().unwrap() {
            '+' | '-' | '*' | '/' => {
                expression.push_back(Term::new_operator(iter.next().unwrap()));
            },
            '0'..='9' => {
                expression.push_back(parse_operand(&mut iter));
            },
            '\n' => break,
            _ => panic!("Invalid character!")
        }
    }

    expression
}


fn parse_operand(iter: &mut Peekable<Chars>) -> Term {
    let mut number_str = String::new();

    while iter.peek().is_some() {
        match iter.peek().unwrap() {
            '0'..='9' => {
                number_str.push(iter.next().unwrap())
            },
            '+' | '-' | '*' | '/' => {
                break;
            },
            '\n' => break,
            _ => panic!("Invalid character!")
        }
    }

    if number_str.is_empty() {
        panic!("Invalid expression!")
    }

    match i64::from_str_radix(number_str.as_str(), 10) {
        Ok(number) => Term::Operand(number),
        Err(_) => panic!("Invalid number!")
    }
}

fn merge_leading_negative(expression: &mut VecDeque<Term>) {
    if let Term::Operator(OperatorKind::Sub) = expression.front().unwrap() {
        expression.pop_front();
        if let Term::Operand(number) = expression.front().unwrap() {
            let neg_number = -number;
            expression.pop_front();
            expression.push_front(Term::Operand(neg_number));
        }
    };
}

fn reduce(expression: &mut VecDeque<Term>) {
    while expression.len() > 1 {
        let expr_front = SingleExpr::from_front(expression);
        let expr_back = SingleExpr::from_back(expression);

        let side = compare_priority(&expr_front, &expr_back);
        match side {
            ExprSide::Front => {
                for _ in 0..3 {
                    expression.pop_front();
                }
                expression.push_front(Term::Operand(expr_front.calculate()));
            },
            ExprSide::Back => {
                for _ in 0..3 {
                    expression.pop_back();
                }
                expression.push_back(Term::Operand(expr_back.calculate()));
            }
        }
    }

}

fn compare_priority(front: &SingleExpr, back: &SingleExpr) -> ExprSide {
    // compare operator precedence
    let precedence_front = front.operator.precedence();
    let precedence_back = back.operator.precedence();
    if precedence_front > precedence_back {
        return ExprSide::Front
    };
    if precedence_front < precedence_back {
        return ExprSide::Back
    };

    // compare calculation result
    let value_front = front.calculate();
    let value_back = back.calculate();
    if value_front > value_back {
        return ExprSide::Front
    };
    if value_front < value_back {
        return ExprSide::Back
    };

    // if every other conditions are the same,
    // front expression has higher priority
    ExprSide::Front
}

fn main() {
    let mut input = String::new();
    match io::stdin().read_line(&mut input) {
        Ok(_) => {
            // println!("{}", input)
        },
        Err(error) => {
            panic!("error: {}", error)
        }
    };
    let mut expression = parse_line(&input);
    merge_leading_negative(&mut expression);

    reduce(&mut expression);
    if let Term::Operand(number) = expression.front().unwrap() {
        println!("{}", number);
    }
}