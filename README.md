# MatrixCalculator_PF

Interactive **console matrix calculator** for Programming Fundamentals (`PF_Project_2`). Operates on square matrices (storage capacity up to 10×10 via `const int n = 10`). Source: `Source.cpp`.

## Overview

After the user enters matrix size and elements, a menu offers identity/symmetry checks, determinant, adjoint, inverse, and solving linear systems \(Ax = b\) for **2×2** and **3×3** cases (formulas are hard-coded for those sizes). Flow control uses labeled `goto` for continue / new matrix / exit.

## Features

Menu options:

1. **Display** the entered matrix  
2. **Symmetric?** — compares matrix to its transpose  
3. **Identity?** — checks principal diagonal ones / off-diagonal zeros (as implemented)  
4. **Determinant** — computed for supported sizes; stored in global `deter`  
5. **Adjoint** — for 2×2 and 3×3  
6. **Inverse** — adjoint/determinant when `deter ≠ 0`  
7. **Solve linear equations** — prompts for a constant vector/matrix and applies inverse-based solution for 2×2 / 3×3  
8. **Terminate**

After each operation: **Y** continue with same matrix, **N** enter a new matrix, **0** exit.

## Tech stack

| Component | Technology |
|-----------|------------|
| Language | C++ |
| I/O formatting | `<iostream>`, `<iomanip>` (`setw`, `left`) |
| IDE | Visual Studio (`PF_Project_2.sln`) |

## Project structure

```
MatrixCalculator_PF/
├── Source.cpp
├── PF_Project_2.sln
├── PF_Project_2.vcxproj
└── PF_Project_2.vcxproj.filters
```

Key functions: `Input_Matrix`, `Output_Matrix`, `Symetric_Matrix`, `Identity_Matrix`, `Determinant_Matrix`, `Adjoint_Matrix`, `Inverse_Matrix`, `Matrix_solution`.

## How to build / run

### Visual Studio

Open `PF_Project_2.sln`, build, and run.

### Command line

```bash
g++ -o MatrixCalculator Source.cpp
./MatrixCalculator
```

## Usage

1. Enter square matrix size (practical use: 2 or 3 for adjoint/inverse/solve).  
2. Enter all \(n²\) integer entries when prompted.  
3. Choose menu option **1–8**.  
4. Follow prompts (e.g. constant terms for option 7).  
5. Choose **Y** / **N** / **0** to continue, restart, or quit.

## How to extend / modify

- Generalize determinant/adjoint/inverse beyond 2×3 using recursion or Gaussian elimination (current code branches on `mat_size`).  
- Use `double` throughout for fractional inverses (many paths print integer divisions).  
- Replace `goto` with loops for clearer control flow.  
- Add matrix addition/multiplication as new menu cases.

## Author

**rohaan2802** — [https://github.com/rohaan2802](https://github.com/rohaan2802)
