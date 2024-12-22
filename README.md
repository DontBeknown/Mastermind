# Mastermind Solver Using Genetic Algorithm  

**Mastermind Solver** is a C program designed to solve the code-breaking puzzle game **Mastermind**. The goal is to find the most efficient method for cracking the code by minimizing the number of guesses required. The solution leverages **Genetic Algorithm (GA)** due to its adaptability, efficiency, and tunable parameters, and compares it with a brute-force **Permutation** approach.  

## Background and Motivation  
Mastermind is a game where players guess a secret color code based on feedback provided for each guess. Solving the game efficiently requires logical reasoning, pattern recognition, and sometimes a bit of luck. This project explores computational strategies to optimize the guessing process by:  
1. Implementing a Genetic Algorithm to simulate and solve the game.  
2. Testing and comparing performance against a brute-force Permutation method.  
3. Measuring performance based on the average number of guesses and processing time.  

## Features  
- **Genetic Algorithm Implementation**  
  - Population-based approach to generate solutions iteratively.  
  - Adjustable parameters to optimize efficiency:  
    - Population size: 60  
    - Generations: 100  
    - Selection method: Proportional Roulette Wheel  
    - Crossover: Single-point (100% chance)  
    - Mutations:  
      - Swap Mutation: 25%  
      - Random Mutation: 75%  
      - Inversion Mutation: 25%  
  - Termination conditions:  
    - Correct code found  
    - Maximum generations reached  
    - Fully selected population  

- **Permutation-based Approach**  
  - Exhaustive search for all possible solutions for benchmarking.  

- **Performance Metrics**  
  - Number of guesses required to find the correct code.  
  - Processing time for the algorithm to complete.  

## How It Works  
1. **Genetic Algorithm**  
   - Initialize a population of potential solutions.  
   - Evaluate fitness based on proximity to the correct code.  
   - Use selection, crossover, and mutation operators to evolve the population.  
   - Repeat until a termination criterion is met.  

2. **Permutation**  
   - Test all possible codes systematically to find the correct one.  

This project highlights the efficiency of Genetic Algorithms for problem-solving and provides insights into parameter optimization for practical applications.  
