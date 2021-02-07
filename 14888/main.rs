// 14888 연산자 끼워넣기

use std::convert::From;
use std::io;

#[derive(Clone)]
enum Operator {
    Add,
    Sub,
    Mul,
    Div,
}

#[derive(Clone)]
struct UnusedOperators {
    add: u8,
    sub: u8,
    mul: u8,
    div: u8,
}

impl From<Vec<u8>> for UnusedOperators {
    fn from(num_of_operators: Vec<u8>) -> Self {
        Self {
            add: num_of_operators[0],
            sub: num_of_operators[1],
            mul: num_of_operators[2],
            div: num_of_operators[3],
        }
    }
}

impl UnusedOperators {
    fn use_add(&self) -> Result<Self, ()> {
        if self.add > 0 {
            let mut yet_unused_operators = self.clone();
            yet_unused_operators.add -= 1;
            Ok(yet_unused_operators)
        } else {
            Err(())
        }
    }

    fn use_sub(&self) -> Result<Self, ()> {
        if self.sub > 0 {
            let mut yet_unused_operators = self.clone();
            yet_unused_operators.sub -= 1;
            Ok(yet_unused_operators)
        } else {
            Err(())
        }
    }

    fn use_mul(&self) -> Result<Self, ()> {
        if self.mul > 0 {
            let mut yet_unused_operators = self.clone();
            yet_unused_operators.mul -= 1;
            Ok(yet_unused_operators)
        } else {
            Err(())
        }
    }

    fn use_div(&self) -> Result<Self, ()> {
        if self.div > 0 {
            let mut yet_unused_operators = self.clone();
            yet_unused_operators.div -= 1;
            Ok(yet_unused_operators)
        } else {
            Err(())
        }
    }

    fn is_empty(&self) -> bool {
        self.add + self.sub + self.mul + self.div == 0
    }
}

fn recursively_push_permutations(
    permutations: &mut Vec<Vec<Operator>>,
    operations_so_far: Vec<Operator>,
    operators_to_use: UnusedOperators,
) {
    if operators_to_use.is_empty() {
        permutations.push(operations_so_far);
    } else {
        if let Ok(unused_ops) = operators_to_use.use_add() {
            let mut child_operations = operations_so_far.clone();
            child_operations.push(Operator::Add);
            recursively_push_permutations(permutations, child_operations, unused_ops);
        }
        if let Ok(unused_ops) = operators_to_use.use_sub() {
            let mut child_operations = operations_so_far.clone();
            child_operations.push(Operator::Sub);
            recursively_push_permutations(permutations, child_operations, unused_ops);
        }
        if let Ok(unused_ops) = operators_to_use.use_mul() {
            let mut child_operations = operations_so_far.clone();
            child_operations.push(Operator::Mul);
            recursively_push_permutations(permutations, child_operations, unused_ops);
        }
        if let Ok(unused_ops) = operators_to_use.use_div() {
            let mut child_operations = operations_so_far.clone();
            child_operations.push(Operator::Div);
            recursively_push_permutations(permutations, child_operations, unused_ops);
        }
    }
}

fn create_operator_permutations(operators_to_use: UnusedOperators) -> Vec<Vec<Operator>> {
    let mut permutations = Vec::new();
    recursively_push_permutations(&mut permutations, Vec::new(), operators_to_use);
    permutations
}

fn main() {
    io::stdin()
        .read_line(&mut String::new())
        .expect("readline failed: ");

    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");
    let operands: Vec<i32> = input
        .trim()
        .split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();
    input.clear();

    io::stdin()
        .read_line(&mut input)
        .expect("readline failed: ");
    let num_of_operators: Vec<u8> = input
        .trim()
        .split_whitespace()
        .map(|word| word.parse().unwrap())
        .collect();
    let unused_operators = UnusedOperators::from(num_of_operators);
    let operator_permutations = create_operator_permutations(unused_operators);

    let calculation_results = operator_permutations.iter().map(|permutation| {
        let mut calculation_result = operands[0];
        permutation
            .iter()
            .enumerate()
            .for_each(|(idx, operator)| match operator {
                Operator::Add => calculation_result += operands[idx + 1],
                Operator::Sub => calculation_result -= operands[idx + 1],
                Operator::Mul => calculation_result *= operands[idx + 1],
                Operator::Div => calculation_result /= operands[idx + 1],
            });
        calculation_result
    });

    println!("{}", calculation_results.clone().max().unwrap());
    println!("{}", calculation_results.clone().min().unwrap());
}
