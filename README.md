
## 1. 8-Puzzle Solver

An interactive solver for the classic 8-puzzle problem, using multiple search algorithms:
Capstone project for UCR's CS170 (Introduction to Artificial Intelligence) Winter 2021 taught by Dr. Eamonn Keogh.

- **Algorithms Supported:**  
  - Uniform Cost Search  
  - A* with Misplaced Tile heuristic  
  - A* with Manhattan Distance heuristic

- **Features:**  
  - User can input custom puzzle configurations or use a default puzzle.  
  - Displays the solution path, nodes expanded, maximum nodes enqueued, and solution depth.  
  - Provides step-by-step output of the best node expanded during search.

- **Usage:**  
  Compile and run the `eight_puzzle_solver.cpp` file. Follow on-screen prompts to input puzzle and select algorithm.

---

## 2. Feature Selection Algorithm

Implements Forward Selection and Backward Elimination to find the best feature subset for classification accuracy using nearest neighbor and leave-one-out cross-validation.

- **Features:**  
  - Reads dataset from user-specified file.  
  - Reports default classification accuracy for baseline comparison.  
  - Displays progress of feature subsets tested and their accuracies.  
  - Warns about potential local maxima during search.

- **Usage:**  
  Compile and run the `feature_selection.cpp` file. Provide dataset filename when prompted and select algorithm.
